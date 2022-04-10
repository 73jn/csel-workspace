cmd_/workspace/src/pilotes/exercice3/modules.order := {   echo /workspace/src/pilotes/exercice3/mymodule.ko; :; } | awk '!x[$$0]++' - > /workspace/src/pilotes/exercice3/modules.order
