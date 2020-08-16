
#ifndef INSTRUMEM_H
#define INSTRUMEM_H

#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "../lib/value-rt/helpers.h"

using namespace llvm;

namespace instrumem
{

    struct InstruMemPass : public llvm::FunctionPass,
                           llvm::InstVisitor<InstruMemPass>
    {

    private:
        const std::string pre = "__InstruMem_";

    public:
        static char ID;
        llvm::Function *F = nullptr;

        // int loadId = 0;
        // int storeId = 0;

        InstruMemPass();

        virtual void getAnalysisUsage(AnalysisUsage &AU) const override
        {
            // AU.addRequired<DataLayoutPass>();
        }

        bool runOnFunction(llvm::Function &f) override;

        llvm::Type *i8PtrTy = nullptr;

        llvm::Type *voidTy = nullptr;
        llvm::Type *i64Ty = nullptr;
        llvm::Type *fTy = nullptr;
        llvm::Type *dTy = nullptr;

        llvm::Value* onLoad = nullptr;
        llvm::Value* onStore = nullptr;
        llvm::Value* onFini = nullptr;
        llvm::Value* onArg = nullptr;
        llvm::Value* onRet = nullptr;

        void visitLoadInst(LoadInst &li);
        void visitStoreInst(StoreInst &si);
        void visitReturnInst(ReturnInst &I);

        void visitFunction(Function &f);

        int GetTypeEnum(llvm::Value *);
    };

} // namespace instrumem

#endif
