cmd_/workspace/src/pilotes/exercice3/Module.symvers := sed 's/\.ko$$/\.o/' /workspace/src/pilotes/exercice3/modules.order | scripts/mod/modpost    -o /workspace/src/pilotes/exercice3/Module.symvers -e -i Module.symvers   -T -