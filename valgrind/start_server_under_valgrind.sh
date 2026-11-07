valgrind \
    --tool=memcheck \
    --leak-check=full \
    --show-leak-kinds=definite,indirect \
    --errors-for-leak-kinds=definite,indirect \
    --track-origins=yes \
    --num-callers=30 \
    --error-markers=VALGRINDERROR-BEGIN,VALGRINDERROR-END \
    --trace-children=yes \
    --error-exitcode=99 \
    --log-file="$HOME/pg-valgrind-logs/%p.log" \
    --suppressions="$PG_SRC/src/tools/valgrind.supp" \
    postgres \
        -D "$PGDATA" \
        -p 65432 \
        -c shared_buffers=64MB \
        -c autovacuum=off