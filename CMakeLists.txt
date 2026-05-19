cmake_minimum_required(VERSION 3.16)
project(DSAlgoDemo VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)

set(SOURCES
    main.cpp
    mainwindow.cpp
    algorithmbase.cpp

    # Chapter 3 - Stack, Queue, Array
    chapter3_stack_queue/bracketmatching.cpp
    chapter3_stack_queue/expressioneval.cpp
    chapter3_stack_queue/recursionstack.cpp
    chapter3_stack_queue/levelorder.cpp
    chapter3_stack_queue/bufferqueue.cpp

    # Chapter 4 - String
    chapter4_string/bruteforce.cpp
    chapter4_string/kmp.cpp
    chapter4_string/kmpnextval.cpp

    # Chapter 5 - Tree & Binary Tree
    chapter5_tree/binarytreetraversal.cpp
    chapter5_tree/threadedtree.cpp
    chapter5_tree/treetransform.cpp
    chapter5_tree/foresttraversal.cpp
    chapter5_tree/huffman.cpp
    chapter5_tree/unionfind.cpp

    # Chapter 6 - Graph
    chapter6_graph/bfs.cpp
    chapter6_graph/dfs.cpp
    chapter6_graph/mst.cpp
    chapter6_graph/shortestpath.cpp
    chapter6_graph/toposort.cpp
    chapter6_graph/criticalpath.cpp

    # Chapter 7 - Search
    chapter7_search/sequentialsearch.cpp
    chapter7_search/binarysearch.cpp
    chapter7_search/blocksearch.cpp
    chapter7_search/bst.cpp
    chapter7_search/avl.cpp
    chapter7_search/rbtree.cpp
    chapter7_search/btree.cpp
    chapter7_search/bplustree.cpp
    chapter7_search/hashtable.cpp

    # Chapter 8 - Sort
    chapter8_sort/insertionsort.cpp
    chapter8_sort/shellsort.cpp
    chapter8_sort/bubblesort.cpp
    chapter8_sort/quicksort.cpp
    chapter8_sort/selectionsort.cpp
    chapter8_sort/heapsort.cpp
    chapter8_sort/mergesort.cpp
    chapter8_sort/radixsort.cpp
    chapter8_sort/countingsort.cpp
    chapter8_sort/externalsort.cpp

    # Visualization
    visual/barchartwidget.cpp
    visual/treewidget.cpp
    visual/graphwidget.cpp
)

set(HEADERS
    algorithmbase.h
    algorithmfactory.h
    mainwindow.h

    chapter3_stack_queue/bracketmatching.h
    chapter3_stack_queue/expressioneval.h
    chapter3_stack_queue/recursionstack.h
    chapter3_stack_queue/levelorder.h
    chapter3_stack_queue/bufferqueue.h

    chapter4_string/bruteforce.h
    chapter4_string/kmp.h
    chapter4_string/kmpnextval.h

    chapter5_tree/binarytreetraversal.h
    chapter5_tree/threadedtree.h
    chapter5_tree/treetransform.h
    chapter5_tree/foresttraversal.h
    chapter5_tree/huffman.h
    chapter5_tree/unionfind.h

    chapter6_graph/bfs.h
    chapter6_graph/dfs.h
    chapter6_graph/mst.h
    chapter6_graph/shortestpath.h
    chapter6_graph/toposort.h
    chapter6_graph/criticalpath.h

    chapter7_search/sequentialsearch.h
    chapter7_search/binarysearch.h
    chapter7_search/blocksearch.h
    chapter7_search/bst.h
    chapter7_search/avl.h
    chapter7_search/rbtree.h
    chapter7_search/btree.h
    chapter7_search/bplustree.h
    chapter7_search/hashtable.h

    chapter8_sort/insertionsort.h
    chapter8_sort/shellsort.h
    chapter8_sort/bubblesort.h
    chapter8_sort/quicksort.h
    chapter8_sort/selectionsort.h
    chapter8_sort/heapsort.h
    chapter8_sort/mergesort.h
    chapter8_sort/radixsort.h
    chapter8_sort/countingsort.h
    chapter8_sort/externalsort.h

    visual/barchartwidget.h
    visual/treewidget.h
    visual/graphwidget.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/chapter3_stack_queue
    ${CMAKE_CURRENT_SOURCE_DIR}/chapter4_string
    ${CMAKE_CURRENT_SOURCE_DIR}/chapter5_tree
    ${CMAKE_CURRENT_SOURCE_DIR}/chapter6_graph
    ${CMAKE_CURRENT_SOURCE_DIR}/chapter7_search
    ${CMAKE_CURRENT_SOURCE_DIR}/chapter8_sort
    ${CMAKE_CURRENT_SOURCE_DIR}/visual
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui)

if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
