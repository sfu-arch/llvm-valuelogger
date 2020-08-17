#define DEBUG_TYPE "lx"

#include "llvm/ADT/SmallString.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/CodeExtractor.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/CallSite.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/InitializePasses.h"

#include "LoopExtractor.h"

#include <iostream>

using namespace llvm;
using namespace std;
using namespace lx;

char TargetLoopExtractor::ID = 0;

bool TargetLoopExtractor::doInitialization(Module &M)
{
    // SmallVector<StringRef, 4> LocList;
    // StringRef(LLVMLXLocations).split(LocList, ",");
    // for(auto Loc : LocList) {
    //     if(Loc.find(":") == StringRef::npos || Loc.find(":") != Loc.rfind(":")) {
    //         report_fatal_error("[llvm-lx] Specify locations in the form of -t=<file1>:<line1>[,<file2>:<line2> ...]");
    //     }
    //     auto File = Loc.substr(0, Loc.find(":"));
    //     auto Line = stoi(Loc.substr(Loc.find(":")+1));
    //     Locations.insert({File, Line});
    // }

    // if(LLVMLXDumpAllLoopLocations)
    //     LoopLocationDumpFile.open("loop-locs.txt", ios::out);

    return false;
}

bool TargetLoopExtractor::doFinalization(Module &M)
{

    for (auto Loc : Locations)
    {
        errs() << "[llvm-lx] Loop Not Found - " << Loc.first << " " << Loc.second << "\n";
    }

    return false;
}

//! Adapted from lib/Transform/IPO/LoopExtractor.cpp
bool TargetLoopExtractor::extractLoop(Loop *L,
                                      LoopInfo &LI, DominatorTree &DT, string Name)
{

    bool ShouldExtractLoop = true;

    if (!L->isLoopSimplifyForm())
    {
        errs() << "[llvm-lx] Loop not in Simplify Form\n";
        ShouldExtractLoop = false;
    }

    // Split all the exit basic blocks to ensure that in case there is a
    // exit basic block which has work done in the block before return, then
    // we should split the block and leave the extra instructions outside the
    // extracted function.

    SmallVector<BasicBlock *, 8> ExitBlocks;
    L->getExitBlocks(ExitBlocks);

    for (unsigned i = 0, e = ExitBlocks.size(); i != e; ++i)
    {
        SplitBlock(ExitBlocks[i], &*ExitBlocks[i]->getFirstInsertionPt(), &DT, &LI);
    }

    if (ShouldExtractLoop)
    {
        // We must omit landing pads. Landing pads must accompany the invoke
        // instruction. But this would result in a loop in the extracted
        // function. An infinite cycle occurs when it tries to extract that loop as
        // well.
        SmallVector<BasicBlock *, 8> ExitBlocks;
        L->getExitBlocks(ExitBlocks);
        for (unsigned i = 0, e = ExitBlocks.size(); i != e; ++i)
        {
            if (ExitBlocks[i]->isLandingPad())
            {
                ShouldExtractLoop = false;
                errs() << "[llvm-lx] Omit Landing Pad Instructions\n";
                break;
            }
        }
    }

    if (ShouldExtractLoop)
    {
        CodeExtractor Extractor(DT, *L);
        Function *F = Extractor.extractCodeRegion();
        if (F)
        {
            F->setName(Name);
            F->addFnAttr(Attribute::NoInline);
            stripDebugInfo(*F);
            ExtractedLoopFunctions.insert(F);
            return true;
        }
    }

    return false;
}

static SetVector<Loop *> getLoops(LoopInfo &LI)
{

    SetVector<Loop *> Loops;

    for (auto &L : LI)
    {
        for (auto &SL : L->getSubLoops())
        {
            Loops.insert(SL);
        }
        Loops.insert(L);
    }

    return Loops;
}

static string getBaseName(string Path)
{
    auto Idx = Path.find_last_of('/');
    return Idx == string::npos ? Path : Path.substr(Idx + 1);
}

static string getLXName(string File, uint64_t Line)
{
    return string("__lx_") + File + string("_") + to_string(Line);
}

bool TargetLoopExtractor::runOnModule(Module &M)
{

    for (auto &F : M)
    {

        if (F.isDeclaration() ||
            ExtractedLoopFunctions.count(&F))
            continue;

        auto &LI = getAnalysis<LoopInfoWrapperPass>(F).getLoopInfo();
        auto &DT = getAnalysis<DominatorTreeWrapperPass>(F).getDomTree();
        auto &SEWP = getAnalysis<ScalarEvolutionWrapperPass>(F).getSE();

        bool loop_extract = false;

        auto &LI = getAnalysis<LoopInfoWrapperPass>(F).getLoopInfo();
        auto loops = getLoops(LI);

        uint64_t loop_cnt = 0;
        for (auto &L : getLoops(LI))
        {
            if (L->getLoopDepth() == 1)
            {
                if (!extractLoop(L, LI, DT, getLXName("loop", loop_cnt)))
                {
                    errs() << "[llvm-lx] Unable to extract loop at  :" << loop_cnt << "\n";
                }else{
                    loop_extract = true;
                }
                loop_cnt++;
            }
        }

        return loop_extract;
    }
