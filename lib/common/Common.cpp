#define DEBUG_TYPE "helpers"

#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/Pass.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

#include "Common.h"

#include <cassert>
#include <cmath>
#include <experimental/iterator>
#include <fstream>
#include <iostream>

using namespace llvm;
using namespace std;

namespace helpers
{
    // LabelUID Helper Class
    char LabelUID::ID = 0;

    RegisterPass<LabelUID> X("lableUID", "Labeling the instructions with UID");

    template <typename T>
    void LabelUID::visitGeneric(std::string S, T &IT)
    {
        if (values.count(&IT) == 0)
        {
            values.insert(make_pair(&IT, counter));
            counter++;
        }
        auto &Context = IT.getContext();
        MDNode *N =
            MDNode::get(Context, MDString::get(Context, to_string(values[&IT])));
        IT.setMetadata(S, N);
    }

    void LabelUID::visitFunction(Function &F)
    {
        if (F.isDeclaration())
            return;
        visitGeneric<Function>("UID", F);
    }

    void LabelUID::visitInstruction(Instruction &I)
    {
        if (auto call_inst = dyn_cast<CallInst>(&I))
        {
            auto called = dyn_cast<Function>(
                CallSite(&I).getCalledValue()->stripPointerCasts());
            if (!called)
            {
                return;
            }
            if (call_inst->getCalledFunction()->isDeclaration())
                return;
        }
        visitGeneric<Instruction>("UID", I);
    }

    void LabelUID::visitBasicBlock(BasicBlock &BB)
    {
        if (values.count(&BB) == 0)
        {
            values.insert(make_pair(&BB, counter));
            counter++;
        }
        auto &Context = BB.getContext();
        MDNode *N =
            MDNode::get(Context, MDString::get(Context, to_string(values[&BB])));
        BB.getTerminator()->setMetadata("UID", N);
    }

    bool LabelUID::runOnFunction(Function &F)
    {
        visit(F);
        return false;
    }
} // namespace helpers
