#!/usr/bin/env perl
use strict;
use warnings;
use IO::Handle;
use JSON;
use Net::WebSocket::Server;

my $in = IO::Handle->new();
$in->fdopen(fileno(STDIN), "r") or die "Can't open stdin!";

my $ws = Net::WebSocket::Server->new(
    listen => 6502,
    tick_period => 1,
    on_tick => sub {
        my ($serv) = @_;

        my $msg = $in->getline;
        if($msg =~ /(\d+)\s+(\d+)\s+(\d+\.?\d*)\s+(\d+\.?\d*)\s+(\d+\.?\d*)/) {
            my $data = encode_json({
                x => $1,
                y => $2,
                r => $3,
                g => $4,
                b => $5
            });

            $_->send_utf8($data) for $serv->connections;
        } else {
            print "Unrecognised line";
        }
    }
);

$ws->start();