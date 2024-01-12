SET (TARGET_NAME s2e2_tests)


SET (HEADERS
    "src/test_utils.hpp"
)

SET (SOURCES
    "src/converter_tests.cpp"
    "src/evaluator_tests.cpp"
    "src/main.cpp"
    "src/tokenizer_tests.cpp"
    "src/functions/function_add_days_tests.cpp"
    "src/functions/function_format_date_tests.cpp"
    "src/functions/function_if_tests.cpp"
    "src/functions/function_now_tests.cpp"
    "src/functions/function_replace_tests.cpp"
    "src/operators/operator_and_tests.cpp"
    "src/operators/operator_equal_tests.cpp"
    "src/operators/operator_greater_or_equal_tests.cpp"
    "src/operators/operator_greater_tests.cpp"
    "src/operators/operator_less_or_equal_tests.cpp"
    "src/operators/operator_less_tests.cpp"
    "src/operators/operator_not_tests.cpp"
    "src/operators/operator_not_equal_tests.cpp"
    "src/operators/operator_or_tests.cpp"
    "src/operators/operator_plus_tests.cpp"
)

ADD_EXECUTABLE (${TARGET_NAME}
    ${HEADERS}
    ${SOURCES}
)

TARGET_LINK_LIBRARIES (${TARGET_NAME}
    s2e2
    gmock
    gtest
)

TARGET_INCLUDE_DIRECTORIES (${TARGET_NAME}
    PRIVATE "../include"
    PRIVATE "../src"
)

INSTALL (
    TARGETS ${TARGET_NAME} 
    RUNTIME DESTINATION ${TEST_OUTPUT_DIR}
)