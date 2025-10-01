#!/usr/bin/perl
use strict;
use warnings;
use CGI qw(:standard);
use File::Temp qw(tempfile tempdir);
use JSON;

# Configuration
my $python_dir = "C:/EDQ-AI/python";      # Folder containing Python AI scripts
my $default_script = "process_edq_ai.py"; # Default Python AI processor
my $temp_dir = tempdir(CLEANUP => 1);

# Read input JSON from form or URL
my $json_input = param('event_json') || '';
if (!$json_input) {
    print header('application/json');
    print encode_json({ error => "No EDQ-AI event data received" });
    exit;
}

# Decode JSON to validate
my $data;
eval { $data = decode_json($json_input); };
if ($@) {
    print header('application/json');
    print encode_json({ error => "Invalid JSON: $@" });
    exit;
}

# Create temp files
my ($fh_in, $tmp_in)   = tempfile(DIR => $temp_dir);
my ($fh_out, $tmp_out) = tempfile(DIR => $temp_dir);

print $fh_in $json_input;
close $fh_in;

# Determine Python script to execute
my $script = param('ai_script') || $default_script;

# Execute Python AI processor
my $cmd = "python \"$python_dir/$script\" \"$tmp_in\" \"$tmp_out\"";
my $output = `$cmd 2>&1`;
if ($? != 0) {
    print header('application/json');
    print encode_json({ error => "Python execution failed", details => $output });
    unlink $tmp_in, $tmp_out;
    exit;
}

# Read Python output
open my $fh, '<', $tmp_out or die "Cannot open $tmp_out: $!";
my $processed_json = do { local $/; <$fh> };
close $fh;

# Clean up temp files
unlink $tmp_in, $tmp_out;

# Generate JS for front-end dashboard
print header('text/html; charset=utf-8');
print <<HTML;
<html>
<head>
<script type="text/javascript">
var edqAIResults = $processed_json;

if (typeof updateEDQDashboard === "function") {
    updateEDQDashboard(edqAIResults);
} else {
    console.log("EDQ-AI Results:", edqAIResults);
}
</script>
</head>
<body>
<h2>EDQ-AI Event Processed</h2>
</body>
</html>
HTML
