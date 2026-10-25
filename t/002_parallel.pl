use POSIX qw(_exit);
use strict;
use warnings FATAL => 'all';

use PostgreSQL::Test::Cluster;
use Test::More;

my $node = PostgreSQL::Test::Cluster->new('parallel_test');

$node->init;
$node->start;

$node->safe_psql('postgres', 'CREATE EXTENSION pg_array_agg');

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
                q{
                    SELECT array_sum(
                        ARRAY[1, 2, 3, 4, 5]::integer[]
                    );
                });

            die "incorrect result: $result"
                unless $result eq '15';
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

$node->stop;

done_testing();
