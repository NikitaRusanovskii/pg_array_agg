use POSIX qw(_exit);
use strict;
use warnings FATAL => 'all';

use PostgreSQL::Test::Cluster;
use Test::More;

my $node = PostgreSQL::Test::Cluster->new('parallel_test');

$node->init;
$node->start;

sub check_parallel {
    my ($query, $expected_result) = @_;

    my @pids;

    for my $worker (1 .. 32)
    {
        my $pid = fork();

        die "error of fork: $!" unless defined $pid;

        if ($pid == 0)
        {
            my $correct = eval {
                my $result = $node->safe_psql(
                    'postgres',
                    $query);

                die "incorrect result: $result"
                    unless $result eq $expected_result;
                1;
            };

            _exit($correct ? 0 : 1);
        }

        push @pids, $pid;
    }

    my $failed = 0;

    for my $pid (@pids)
    {
        waitpid($pid, 0);
        $failed++ if $? != 0;
    }

    is(
        $failed,
        0,
        'all workers has correct results'
    );
}

$node->safe_psql('postgres', 'CREATE EXTENSION pg_array_agg');

check_parallel(
    q{
        SELECT array_product(
            ARRAY[1, 2, 3, 4, 5]::integer[]
        );
    },
    '120'
);

check_parallel(
    q{
        SELECT array_sum(
            ARRAY[1, 2, 3, 4, 5]::integer[]
        );
    },
    '15'
);

check_parallel(
    q{
        SELECT array_avg(
            ARRAY[1, 2, 3, 4, 5]::integer[]
        );
    },
    '3.0000000000000000'
);

check_parallel(
    q{
        SELECT array_min(
            ARRAY[1, 2, 3, 4, 5]::integer[]
        );
    },
    '1'
);

check_parallel(
    q{
        SELECT array_max(
            ARRAY[1, 2, 3, 4, 5]::integer[]
        );
    },
    '5'
);

check_parallel(
    q{
        SELECT array_count(
            ARRAY[1, 2, 3, 4, 5]::integer[], 3
        );
    },
    '1'
);

check_parallel(
    q{
        SELECT array_count_if(
            ARRAY[1, 2, 3, 4, 5]::integer[], 'x > 0'
        );
    },
    '5'
);

check_parallel(
    q{
        SELECT array_any(
            ARRAY[1, 2, 3, 4, 5]::integer[], 'x == 3'
        );
    },
    't'
);

check_parallel(
    q{
        SELECT array_all(
            ARRAY[1, 2, 3, 4, 5]::integer[], 'x < 10'
        );
    },
    't'
);

check_parallel(
    q{
        SELECT array_none(
            ARRAY[1, 2, 3, 4, 5]::integer[], 'x == 7'
        );
    },
    't'
);

$node->stop;

done_testing();