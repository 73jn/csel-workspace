cmd_/workspace/src/pilotes/exercice2/modules.order := {   echo /workspace/src/pilotes/exercice2/mymodule.ko; :; } | awk '!x[$$0]++' - > /workspace/src/pilotes/exercice2/modules.order
