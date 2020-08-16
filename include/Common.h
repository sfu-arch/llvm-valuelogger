#pragma once

#include "llvm/ADT/DenseSet.h"
#include "llvm/ADT/SetVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/IR/CallSite.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/CommandLine.h"

#include <map>
#include <sstream>
#include <string>

using namespace llvm;

namespace helpers
{

    class LabelUID : public FunctionPass, public InstVisitor<LabelUID>
    {
        friend class InstVisitor<LabelUID>;

        uint64_t counter;

        void visitFunction(Function &F);

        void visitBasicBlock(BasicBlock &BB);

        void visitInstruction(Instruction &I);

        template <typename T>
        void visitGeneric(std::string, T &);

        std::map<Value *, uint64_t> values;

    public:
        static char ID;

        LabelUID() : FunctionPass(ID), counter(0) {}

        bool doInitialization(Module &) override
        {
            counter = 0;
            values.clear();
            return false;
        };

        bool doFinalization(Module &) override { return true; };

        bool runOnFunction(Function &) override;

        void getAnalysisUsage(AnalysisUsage &AU) const override
        {
            AU.setPreservesAll();
        }
    };

    // LabelUID Helper Class
    // char LabelUID::ID = 0;

    // RegisterPass<LabelUID> X("lableUID", "Labeling the instructions with UID");

    // template <typename T>
    // void LabelUID::visitGeneric(std::string S, T &IT)
    // {
    //     if (values.count(&IT) == 0)
    //     {
    //         values.insert(make_pair(&IT, counter));
    //         counter++;
    //     }
    //     auto &Context = IT.getContext();
    //     MDNode *N =
    //         MDNode::get(Context, MDString::get(Context, to_string(values[&IT])));
    //     IT.setMetadata(S, N);
    // }
} // namespace helpers