cmd_/workspace/src/pilotes/exercice5/modules.order := {   echo /workspace/src/pilotes/exercice5/mymodule.ko; :; } | awk '!x[$$0]++' - > /workspace/src/pilotes/exercice5/modules.order
