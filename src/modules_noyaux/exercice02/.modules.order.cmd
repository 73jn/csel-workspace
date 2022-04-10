cmd_/workspace/src/exercice02/modules.order := {   echo /workspace/src/exercice02/mymodule.ko; :; } | awk '!x[$$0]++' - > /workspace/src/exercice02/modules.order
