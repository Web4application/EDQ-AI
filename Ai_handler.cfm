<cfsetting enablecfoutputonly="true">
<cfcontent type="application/json; charset=utf-8">

<cftry>
    <!--- Input Validation --->
    <cfparam name="form.event_json" default="">
    <cfparam name="form.ai_script" default="process_edq_ai.py">

    <cfif Len(Trim(form.event_json)) EQ 0>
        <cfthrow message="Missing required parameter: event_json" type="BadRequest">
    </cfif>

    <!--- Prepare Paths --->
    <cfset tempDir = GetTempDirectory()>
    <cfset tempfileIn  = GetTempFile(tempDir, "edq_ai_input_")>
    <cfset tempfileOut = GetTempFile(tempDir, "edq_ai_output_")>
    <cfset pythonScript = expandPath("/python/#form.ai_script#")>

    <!--- Security Check --->
    <cfif NOT FileExists(pythonScript)>
        <cfthrow message="Python script not found: #form.ai_script#" type="FileNotFound">
    </cfif>

    <!--- Write input JSON --->
    <cffile action="write" file="#tempfileIn#" output="#form.event_json#" charset="utf-8">

    <!--- Run Python Script --->
    <cfset execResult = "">
    <cfexecute name="python"
               arguments="#pythonScript# #tempfileIn# #tempfileOut#"
               timeout="180"
               variable="execResult">
    </cfexecute>

    <!--- Read AI Output --->
    <cfif FileExists(tempfileOut)>
        <cffile action="read" file="#tempfileOut#" variable="aiResults" charset="utf-8">
        <cfset resultsStruct = deserializeJSON(aiResults)>
    <cfelse>
        <cfthrow message="No output returned from Python script." type="ExecutionError">
    </cfif>

    <!--- Build Success Response --->
    <cfset response = {
        status = "success",
        data = resultsStruct,
        exec_log = execResult,
        timestamp = now()
    }>

    <cfoutput>#serializeJSON(response)#</cfoutput>

    <!--- Cleanup --->
    <cffile action="delete" file="#tempfileIn#" />
    <cffile action="delete" file="#tempfileOut#" />

<cfcatch type="any">
    <!--- Error Handling --->
    <cfset errorResponse = {
        status = "error",
        message = cfcatch.message,
        detail = cfcatch.detail ?: "",
        type = cfcatch.type,
        timestamp = now()
    }>
    <cfheader statuscode="500" statustext="Internal Server Error">
    <cfoutput>#serializeJSON(errorResponse)#</cfoutput>
</cfcatch>
</cftry>

<cfsetting enablecfoutputonly="false">
