cmd_/workspace/src/exercice06/Module.symvers := sed 's/\.ko$$/\.o/' /workspace/src/exercice06/modules.order | scripts/mod/modpost    -o /workspace/src/exercice06/Module.symvers -e -i Module.symvers   -T -
