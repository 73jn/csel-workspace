cmd_/workspace/src/pilotes/exercice7/modules.order := {   echo /workspace/src/pilotes/exercice7/mymodule.ko; :; } | awk '!x[$$0]++' - > /workspace/src/pilotes/exercice7/modules.order
