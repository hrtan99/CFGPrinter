#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Support/GraphWriter.h"

using namespace llvm;

namespace {

struct DomTreeGraphWriterPass : public FunctionPass {
  static char ID;
  DomTreeGraphWriterPass() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    DominatorTree DT(F);
    std::string Filename = F.getName().str() + "_domtree.dot";
    std::error_code EC;
    raw_fd_ostream File(Filename, EC, sys::fs::OF_Text);
    if (EC) {
      errs() << "Could not open file: " << EC.message();
      return false;
    }

    // Write the Dominate Tree to the GraphViz file
    WriteGraph(File, &DT);

    return false;
  }
};

} // end anonymous namespace

char DomTreeGraphWriterPass::ID = 0;
static RegisterPass<DomTreeGraphWriterPass> X("dot-domtree", "Dominator Tree Graph Writer Pass");

