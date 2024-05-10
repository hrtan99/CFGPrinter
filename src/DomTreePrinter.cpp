#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/GraphWriter.h"

#include "DomTreePrinter.h"


using namespace llvm;


static cl::opt<bool> Wave("wave-goodbye", cl::init(false), cl::desc("wave good bye"));

PreservedAnalyses DomTreeGraphWriterPass::run(Function &F, FunctionAnalysisManager &AM) {
    errs() << F.getName() << "\n";
    DominatorTree DT(F);
    std::string Filename = "domtree_" + F.getName().str() + ".dot";
    std::error_code EC;
    raw_fd_ostream File(Filename, EC, sys::fs::OF_Text);
    if (EC) {
        errs() << "Could not open file: " << EC.message();
        return PreservedAnalyses::all();
    }
    // Write the Dominate Tree to the GraphViz file
    WriteGraph(File, &DT);
    return PreservedAnalyses::all();
}


