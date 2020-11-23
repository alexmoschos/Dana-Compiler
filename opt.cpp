#include "opt.h"

using namespace llvm;
legacy::PassManager Optimize() {
    legacy::PassManager pm;
    pm.add(createConstantPropagationPass());
    pm.add(createConstantHoistingPass());
    pm.add(createLoopUnrollPass());
    pm.add(createLoopSimplifyCFGPass());
    pm.add(createReassociatePass());
    pm.add(createNewGVNPass());
    pm.add(createCFGSimplificationPass());
    pm.add(createDeadCodeEliminationPass());
    pm.add(createDeadInstEliminationPass());
    for (int i = 0; i < 5; ++i)
        pm.add(createDeadStoreEliminationPass());
    pm.add(createAggressiveDCEPass());
    pm.add(createPrintModulePass(outs()));
    return pm;
}