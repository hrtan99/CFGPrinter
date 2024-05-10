#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/GraphWriter.h"


#include "DomTreePrinter.h"
#include "HeatCallPrinter.h"
#include "HeatCFGPrinter.h"


using namespace llvm;

/* New PM Registration */
llvm::PassPluginLibraryInfo getCFGPrinterPluginInfo() {
    return {
        LLVM_PLUGIN_API_VERSION, "DomTreeGraphWriter", LLVM_VERSION_STRING,
        [](PassBuilder &PB) {
            // PB.registerVectorizerStartEPCallback(
            //     [](llvm::FunctionPassManager &PM, OptimizationLevel Level) {
            //       PM.addPass(DomTreeGraphWriterPass());
            //     });
            PB.registerPipelineParsingCallback(
                [](StringRef Name, llvm::FunctionPassManager &PM, ArrayRef<llvm::PassBuilder::PipelineElement>) {
                    if (Name == "dot-domtree") {
                        PM.addPass(DomTreeGraphWriterPass());
                        return true;
                    }
                    return false;
                }
            );
            PB.registerPipelineParsingCallback(
                [](StringRef Name, llvm::ModulePassManager &PM, ArrayRef<llvm::PassBuilder::PipelineElement>) {
                    if (Name == "dot-heat-callgraph") {
                        PM.addPass(HeatCallGraphDOTPrinterPass());
                        return true;
                    }
                    if (Name == "dot-heat-cfg") {
                        PM.addPass(HeatCFGPrinterPass());
                        return true;
                    }
                    if (Name == "dot-heat-cfg-only") {
                        PM.addPass(HeatCFGOnlyPrinterPass());
                        return true;
                    }
                    return false;
                }
            );
        }
    };

}


// This is the core interface for pass plugins. It guarantees that 'opt' will
// be able to recognize DomTreeGraphWriterPass when added to the pass pipeline on the
// command line, i.e. via '-passes=hello-world'
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
    return getCFGPrinterPluginInfo();
}