<?php
header('Content-type: text/html; charset=utf-8');

// --- CONFIGURATION ---
$python_dir     = "C:/EDQ-AI/python";       // Folder containing Python AI scripts
$default_script = "process_edq_ai.py";      // Default Python AI processor
$tempfile_dir   = sys_get_temp_dir();       // Temporary directory

// --- INPUT HANDLING ---
$event_json = isset($_POST['event_json']) ? $_POST['event_json'] : '';
if (!$event_json) {
    echo json_encode(['error' => 'No EDQ-AI event data received']);
    exit;
}

// Validate JSON
$data = json_decode($event_json, true);
if (json_last_error() !== JSON_ERROR_NONE) {
    echo json_encode(['error' => 'Invalid JSON: ' . json_last_error_msg()]);
    exit;
}

// --- CREATE TEMP FILES ---
$tmp_in  = tempnam($tempfile_dir, 'edq_ai_in_');
$tmp_out = tempnam($tempfile_dir, 'edq_ai_out_');
file_put_contents($tmp_in, $event_json);

// Determine Python script
$script = isset($_POST['ai_script']) ? $_POST['ai_script'] : $default_script;

// --- EXECUTE PYTHON AI PROCESSOR ---
$cmd = "python \"$python_dir/$script\" \"$tmp_in\" \"$tmp_out\"";
$output = shell_exec("$cmd 2>&1");
if (!file_exists($tmp_out)) {
    echo json_encode(['error' => 'Python execution failed', 'details' => $output]);
    unlink($tmp_in);
    exit;
}

// --- READ PYTHON OUTPUT ---
$processed_json = file_get_contents($tmp_out);

// --- CLEAN UP TEMP FILES ---
unlink($tmp_in);
unlink($tmp_out);

// --- OUTPUT TO DASHBOARD ---
?>
<html>
<head>
<script type="text/javascript">
// EDQ-AI Results for front-end
var edqAIResults = <?php echo $processed_json; ?>;

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
