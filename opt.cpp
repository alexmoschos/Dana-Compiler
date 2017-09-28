#include "opt.h"

using namespace llvm;
legacy::PassManager Optimize() {
    legacy::PassManager pm;
    pm.add(createConstantPropagationPass());
    pm.add(createConstantHoistingPass());
    pm.add(createPromoteMemoryToRegisterPass());
    // pm.add(createLoopInstSimplifyPass());
    pm.add(createLoopUnrollPass());
    pm.add(createLoadCombinePass());
    pm.add(createLoopSimplifyPass());
    pm.add(createReassociatePass());
    pm.add(createGVNPass());
    pm.add(createCFGSimplificationPass());
    pm.add(createDeadCodeEliminationPass());
    pm.add(createDeadInstEliminationPass());
    for (int i = 0; i < 5; ++i)
        pm.add(createDeadStoreEliminationPass());
    pm.add(createPromoteMemoryToRegisterPass());
    pm.add(createAggressiveDCEPass());
    pm.add(createLCSSAPass());
    pm.add(createPrintModulePass(outs()));
    // pm.doInitialization();
    return pm;
}