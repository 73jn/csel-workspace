cmd_/workspace/src/exercice08/Module.symvers := sed 's/\.ko$$/\.o/' /workspace/src/exercice08/modules.order | scripts/mod/modpost    -o /workspace/src/exercice08/Module.symvers -e -i Module.symvers   -T -