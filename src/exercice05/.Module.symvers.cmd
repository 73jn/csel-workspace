cmd_/workspace/src/exercice05/Module.symvers := sed 's/\.ko$$/\.o/' /workspace/src/exercice05/modules.order | scripts/mod/modpost    -o /workspace/src/exercice05/Module.symvers -e -i Module.symvers   -T -