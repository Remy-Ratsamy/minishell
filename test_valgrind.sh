#!/bin/bash

CMD_FILE="commands.txt"
RESULT_FILE="valgrind_results.txt"

> "$RESULT_FILE"
LEAK_COUNT=0

while IFS= read -r CMD
do
    echo "Testing: $CMD"

    VALGRIND_OUT=$(printf "%s\nexit\n" "$CMD" | valgrind --leak-check=full \
                         --show-leak-kinds=all \
                         --track-origins=yes \
                         --error-exitcode=42 \
                         --suppressions=readline.supp \
                         ./minishell 2>&1)

    # On ne garde que les leaks non nuls
    LEAKS=$(echo "$VALGRIND_OUT" | grep -E "still reachable|definitely lost|indirectly lost" | grep -v "0 bytes")

    if [ -n "$LEAKS" ]; then
        echo "❌ LEAK détecté pour la commande: $CMD"
        echo "=============================" >> "$RESULT_FILE"
        echo "CMD: $CMD" >> "$RESULT_FILE"
        echo "$LEAKS" >> "$RESULT_FILE"
        echo "=============================" >> "$RESULT_FILE"
        ((LEAK_COUNT++))
    else
        echo "✅ OK"
    fi

done < "$CMD_FILE"

echo ""
echo "Done. $LEAK_COUNT leak(s) détecté(s). Voir $RESULT_FILE"