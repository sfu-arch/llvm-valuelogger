//===- InstruMem.cpp ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements an LLVM memory operation instrumentation pass.
//
//===----------------------------------------------------------------------===//

#include "InstruMem.h"

using namespace instrumem;

InstruMemPass::InstruMemPass() : FunctionPass(ID) {} // InstruMemPass

bool InstruMemPass::runOnFunction(Function &f)
{

    F = &f;
    visit(f);
    return true;
}

void InstruMemPass::visitFunction(Function &f)
{

    auto *m = f.getParent();

    voidTy = Type::getVoidTy(m->getContext());
    i64Ty = Type::getInt64Ty(m->getContext());
    i8PtrTy = Type::getInt8PtrTy(m->getContext());
    fTy = Type::getFloatTy(m->getContext());
    dTy = Type::getDoubleTy(m->getContext());

    onLoad =
        m->getOrInsertFunction(pre + "load", FunctionType::get(
                                                 voidTy, {i8PtrTy, i64Ty, i64Ty, i64Ty}, false))
            .getCallee();
    onStore =
        m->getOrInsertFunction(pre + "store", FunctionType::get(
                                                  voidTy, {i8PtrTy, i64Ty, i64Ty, i64Ty}, false))
            .getCallee();

    onFini =
        m->getOrInsertFunction("__Fini", FunctionType::get(
                                             voidTy, {}, false))
            .getCallee();

    onArg =
        m->getOrInsertFunction("__PrintArg", FunctionType::get(
                                                 voidTy, {i8PtrTy, i64Ty, i64Ty}, false))
            .getCallee();

    onRet =
        m->getOrInsertFunction("__PrintRet", FunctionType::get(
                                                 voidTy, {i8PtrTy, i64Ty}, false))
            .getCallee();
}

void InstruMemPass::visitLoadInst(LoadInst &li)
{

    auto *i64Ty = Type::getInt64Ty(li.getContext());

    Value *loaded = li.getPointerOperand();
    BitCastInst *bc = new BitCastInst(loaded, i8PtrTy, "", &li);

    auto id_value = getUID(li);

    if (!li.isTerminator())
    {

        auto DL = li.getModule()->getDataLayout();
        auto Sz = DL.getTypeStoreSize(cast<PointerType>(li.getPointerOperand()->getType())->getElementType());
        Value *args[] = {bc, ConstantInt::get(i64Ty, Sz),
                         ConstantInt::get(i64Ty, GetTypeEnum(&li)), 
                         ConstantInt::get(i64Ty, id_value)};

        CallInst::Create(onLoad, args)->insertAfter(&li);
    } // load NOT at end of BB

} // visitLoad

void InstruMemPass::visitStoreInst(StoreInst &si)
{

    //int storeId = 0;

    auto *i64Ty = Type::getInt64Ty(si.getContext());

    Value *stored = si.getPointerOperand();
    BitCastInst *bc = new BitCastInst(stored, i8PtrTy, "", &si);

    auto id_value = getUID(si);

    if (!si.isTerminator())
    {

        auto DL = si.getModule()->getDataLayout();
        auto Sz = DL.getTypeStoreSize(cast<PointerType>(si.getPointerOperand()->getType())->getElementType());
        Value *args[] = {bc, ConstantInt::get(i64Ty, Sz),
                         ConstantInt::get(i64Ty, GetTypeEnum(si.getValueOperand())),
                         ConstantInt::get(i64Ty, id_value)};

        CallInst::Create(onStore, args)->insertAfter(&si);
    } // store NOT at end of BB

} // visitStore

void InstruMemPass::visitReturnInst(ReturnInst &I)
{

    for (auto aI = F->arg_begin(); aI != F->arg_end(); ++aI)
    {
        auto argVal = aI;

        auto argI = new AllocaInst(argVal->getType(), Type::getInt8PtrTy(I.getContext())->getPointerAddressSpace(), "", &I);
        auto sI = new StoreInst(argVal, argI, &I);
        auto stored = sI->getPointerOperand();
        auto bc = new BitCastInst(stored, i8PtrTy, "", sI);
        Value *args[] = {bc, ConstantInt::get(Type::getInt64Ty(I.getContext()), aI->getArgNo()), ConstantInt::get(i64Ty, GetTypeEnum(argVal))};

        CallInst::Create(onArg, args)->insertAfter(sI);
    }

    Value *retVal = I.getReturnValue();
    if (retVal != NULL)
    {
        auto aI = new AllocaInst(retVal->getType(), Type::getInt8PtrTy(I.getContext())->getPointerAddressSpace(), "", &I);
        auto sI = new StoreInst(retVal, aI, &I);
        auto stored = sI->getPointerOperand();
        auto bc = new BitCastInst(stored, i8PtrTy, "", sI);
        Value *args[] = {bc, ConstantInt::get(i64Ty, GetTypeEnum(sI->getValueOperand()))};

        CallInst::Create(onRet, args)->insertAfter(sI);
    }

    CallInst::Create(onFini, "", &I);

} //visitReturnInst

int InstruMemPass::GetTypeEnum(Value *v)
{
    if (v->getType()->isFloatTy())
        return FLT;
    else if (v->getType()->isDoubleTy())
        return DBL;
    else if (v->getType()->isIntegerTy(1))
        return BOL;
    else if (v->getType()->isIntegerTy(8))
        return CHR;
    else if (v->getType()->isIntegerTy(16))
        return HLF;
    else if (v->getType()->isIntegerTy(32))
        return WHL;
    else if (v->getType()->isIntegerTy(64))
        return LNG;
    else if (v->getType()->isPointerTy())
        return PTR;

    return INVALID;
} //gettypeenum

char InstruMemPass::ID = 0;
static RegisterPass<InstruMemPass> X("instrumem", "InstruMem Pass");
