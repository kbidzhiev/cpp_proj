if(NOT DEFINED APP OR NOT DEFINED INPUT_FILE OR NOT DEFINED ANSWER_FILE)
  message(FATAL_ERROR "APP, INPUT_FILE, and ANSWER_FILE must be provided")
endif()

if(NOT EXISTS "${INPUT_FILE}")
  message(FATAL_ERROR "Input file not found: ${INPUT_FILE}")
endif()

if(NOT EXISTS "${ANSWER_FILE}")
  message(FATAL_ERROR "Answer file not found: ${ANSWER_FILE}")
endif()

file(READ "${ANSWER_FILE}" expected_raw)
string(STRIP "${expected_raw}" expected)

execute_process(
  COMMAND "${APP}"
  INPUT_FILE "${INPUT_FILE}"
  OUTPUT_VARIABLE actual_raw
  ERROR_VARIABLE app_stderr
  RESULT_VARIABLE app_status
)

if(NOT app_status EQUAL 0)
  message(FATAL_ERROR "App failed with exit code ${app_status}\n${app_stderr}")
endif()

string(STRIP "${actual_raw}" actual)

if(NOT actual STREQUAL expected)
  message(FATAL_ERROR
    "E2E output mismatch for ${INPUT_FILE}\n"
    "Expected: [${expected}]\n"
    "Actual:   [${actual}]"
  )
endif()
