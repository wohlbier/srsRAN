# jgw
add_library(AtlasPasses SHARED IMPORTED GLOBAL)
set_target_properties(AtlasPasses PROPERTIES IMPORTED_LOCATION /home/TraceAtlas/build/lib/AtlasPasses.so)
add_library(AtlasBackend SHARED IMPORTED GLOBAL)
set_target_properties(AtlasBackend PROPERTIES IMPORTED_LOCATION /home/TraceAtlas/build/lib/libAtlasBackend.so)
set(LLVM_INSTALL_PREFIX /home/llvm-9.0.1/build)

# responsible for injecting the tracer. Fairly fragile so be careful
function(InjectTracer tar)
    target_compile_options(${tar} PRIVATE "-flto" "-g")
    set_target_properties(${tar} PROPERTIES LINK_FLAGS "-fuse-ld=lld -Wl,--plugin-opt=emit-llvm")
    add_custom_command(OUTPUT opt.bc
    COMMAND ${LLVM_INSTALL_PREFIX}/bin/opt -load $<TARGET_FILE:AtlasPasses> -Markov $<TARGET_FILE:${tar}> -o opt.bc
 DEPENDS $<TARGET_FILE:${tar}>
    )
    set_source_files_properties(
    opt.bc
        PROPERTIES
        EXTERNAL_OBJECT true
        GENERATED true
    )
    add_executable(${tar}-trace opt.bc)
    target_compile_options(${tar} PRIVATE "-flto" "-g")
    set_target_properties(${tar}-trace PROPERTIES LINKER_LANGUAGE CXX)
    get_target_property(TARGET_LINKS ${tar} LINK_LIBRARIES)
    target_link_libraries(${tar}-trace PRIVATE AtlasBackend ${PAPI_LIBRARIES} ${TARGET_LINKS})
endfunction()

function(InjectMemory tar)
    target_compile_options(${tar} PRIVATE "-flto" "-g")
    set_target_properties(${tar} PROPERTIES LINK_FLAGS "-fuse-ld=lld -Wl,--plugin-opt=emit-llvm")
    add_custom_command(OUTPUT ${tar}.memory.bc
        COMMAND ${LLVM_INSTALL_PREFIX}/bin/opt -load $<TARGET_FILE:AtlasPasses>
 -Memory $<TARGET_FILE:${tar}> -o ${tar}.memory.bc
        DEPENDS $<TARGET_FILE:${tar}>
    )
    set_source_files_properties(
        ${tar}.memory.bc
        PROPERTIES
        EXTERNAL_OBJECT true
        GENERATED true
    )
    add_executable(${tar}-memory-trace ${tar}.memory.bc)
    set_target_properties(${tar}-memory-trace PROPERTIES LINKER_LANGUAGE CXX)
    get_target_property(TARGET_LINKS ${tar} LINK_LIBRARIES)
    target_link_libraries(${tar}-memory-trace PRIVATE AtlasBackend ${PAPI_LIBRARIES} $endfunction()

function(InjectInstance tar)
    target_compile_options(${tar} PRIVATE "-flto" "-g")
    set_target_properties(${tar} PROPERTIES LINK_FLAGS "-fuse-ld=lld -Wl,--plugin-opt=emit-llvm")
    add_custom_command(OUTPUT instance.bc
        COMMAND ${LLVM_INSTALL_PREFIX}/bin/opt -load $<TARGET_FILE:AtlasPasses>
-Instance $<TARGET_FILE:${tar}> -o instance.bc
        DEPENDS $<TARGET_FILE:${tar}>
    )
    set_source_files_properties(
        instance.bc
        PROPERTIES
        EXTERNAL_OBJECT true
        GENERATED true
    )
    add_executable(${tar}-instance-trace instance.bc)
    set_target_properties(${tar}-instance-trace PROPERTIES LINKER_LANGUAGE CXX)
    get_target_property(TARGET_LINKS ${tar} LINK_LIBRARIES)
    target_link_libraries(${tar}-instance-trace PRIVATE AtlasBackend ${PAPI_LIBRARIES} $endfunction()

function(MemProfiler tar)
    target_compile_options(${tar} PRIVATE "-flto" "-g")
    set_target_properties(${tar} PROPERTIES LINK_FLAGS "-fuse-ld=lld -Wl,--plugin-opt=emit-llvm")
    add_custom_command(OUTPUT optMem.bc
        COMMAND ${LLVM_INSTALL_PREFIX}/bin/opt -load $<TARGET_FILE:AtlasPasses> -MemProfile  $<TARGET_FILE:${tar}> -o optMem.bc
        DEPENDS $<TARGET_FILE:${tar}>
    )
    set_source_files_properties(
        optMem.bc
        PROPERTIES
        EXTERNAL_OBJECT true
        GENERATED true
    )
    add_executable(${tar}-memProfile optMem.bc)
    set_target_properties(${tar}-memProfile PROPERTIES LINKER_LANGUAGE CXX)
    get_target_property(TARGET_LINKS ${tar} LINK_LIBRARIES)
    target_link_libraries(${tar}-memProfile PRIVATE AtlasBackend ${PAPI_LIBRARIES} $endfunction()

function(LoopDetection tar)
    target_compile_options(${tar} PRIVATE "-flto" "-g")
    set_target_properties(${tar} PROPERTIES LINK_FLAGS "-fuse-ld=lld -Wl,--plugin-opt=emit-llvm")
    add_custom_command(OUTPUT loopDetect.bc
        COMMAND ${LLVM_INSTALL_PREFIX}/bin/opt -load $<TARGET_FILE:AtlasPasses> -CFG  $<TARGET_FILE:${tar}> -o loopDetect.bc > out 2> loopPrint.txt
        DEPENDS $<TARGET_FILE:${tar}>
    )
    set_source_files_properties(
        loopDetect.bc
        PROPERTIES
        EXTERNAL_OBJECT true
        GENERATED true
    )
    add_executable(${tar}-CFG loopDetect.bc)
    set_target_properties(${tar}-CFG PROPERTIES LINKER_LANGUAGE CXX)
    get_target_property(TARGET_LINKS ${tar} LINK_LIBRARIES)
    target_link_libraries(${tar}-CFG PRIVATE AtlasBackend ${PAPI_LIBRARIES} $endfunction()

function(InjectLastWriter tar)
    target_compile_options(${tar} PRIVATE "-flto" "-g")
    set_target_properties(${tar} PROPERTIES LINK_FLAGS "-fuse-ld=lld -Wl,--plugin-opt=emit-llvm")
    add_custom_command(OUTPUT LastWriter.bc
        COMMAND ${LLVM_INSTALL_PREFIX}/bin/opt -load $<TARGET_FILE:AtlasPasses>
 -LastWriter $<TARGET_FILE:${tar}> -o LastWriter.bc
        DEPENDS $<TARGET_FILE:${tar}>
    )
    set_source_files_properties(
        LastWriter.bc
        PROPERTIES
        EXTERNAL_OBJECT true
        GENERATED true
    )
    add_executable(${tar}-LastWriter-trace LastWriter.bc)
    set_target_properties(${tar}-LastWriter-trace PROPERTIES LINKER_LANGUAGE CXX)
    get_target_property(TARGET_LINKS ${tar} LINK_LIBRARIES)
    target_link_libraries(${tar}-LastWriter-trace PRIVATE AtlasBackend ${PAPI_LIBRARIES} $endfunction()
