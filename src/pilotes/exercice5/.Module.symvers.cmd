cmd_/workspace/src/pilotes/exercice5/Module.symvers := sed 's/\.ko$$/\.o/' /workspace/src/pilotes/exercice5/modules.order | scripts/mod/modpost    -o /workspace/src/pilotes/exercice5/Module.symvers -e -i Module.symvers   -T -
