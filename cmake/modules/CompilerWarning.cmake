function(fuse_target_set_compiler_flags target)
    # Enable some warning based on
    # https://github.com/cpp-best-practices/cppbestpractices/blob/master/02-Use_the_Tools_Available.md

    if(NOT TARGET ${target})
        message(FATAL_ERROR "Target ${target} does not exist.")
    endif()

    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(
            ${target}
            PRIVATE
                /W4          # Enable warning level 4
                /permissive-
                /w14061 # C4061: enumerator 'identifier' in switch of enum 'enumeration' is not explicitly handled by a case label
                /w14062 # C4062: enumerator 'identifier' in switch of enum 'enumeration' is not handled
                #/w14191 # C4191: 'operator': unsafe conversion from 'type_of_expression' to 'type_required'
                /w14242 # C4242: 'identifier': conversion from 'type1' to 'type2', possible loss of data
                /w14254 # C4254: 'operator': conversion from 'type1' to 'type2', possible loss of data
                /w14263 # C4263: 'function': member function does not override any base class virtual member function
                /w14265 # C4265: 'class': class has virtual functions, but destructor is not virtual
                /w14287 # C4287: 'operator': unsigned/negative constant mismatch
                /w14289 # C4289: nonstandard extension used : 'var' : loop control variable declared in the for-loop is used outside the for-loop scope
                /w14296 # C4296: 'operator': expression is always false
                /w14311 # C4311: 'variable': pointer truncation from 'type' to 'type'
                /w14365 # C4365: 'action': conversion from 'type_1' to 'type_2', signed/unsigned mismatch
                /w14545 # C4545: expression before comma evaluates to a function which is missing an argument list
                /w14546 # C4546: function call before comma missing argument list
                /w14547 # C4547: 'operator': operator before comma has no effect; expected operator with side-effect
                /w14549 # C4549: 'operator1': operator before comma has no effect; did you intend 'operator2'?
                /w14555 # C4555: expression has no effect; expected expression with side-effect
                /w14619 # C4619: #pragma warning: there is no warning number 'number'
                /w14640 # C4640: 'instance': construction of local static object is not thread-safe
                /w14668 # C4668: 'symbol' is not defined as a preprocessor macro, replacing with '0' for 'directives'
                /w14826 # C4826: Conversion from 'type1' to 'type2' is sign-extended. This may cause unexpected runtime behavior.
                /w14905 # C4905: wide string literal cast to 'LPSTR'
                /w14906 # C4906: string literal cast to 'LPWSTR'
                /w14928 # C4928: illegal copy-initialization; more than one user-defined conversion has been implicitly applied
                /w15262 # C5262: implicit fall-through occurs here; are you missing a break statement
                /w15264 # C5264: 'variable-name': 'const' variable is not used

                # Uncomment to find now warning
                #/Wall
                #/wd4514 # C4514: 'function': unreferenced inline function has been removed
                #/wd4626 # C4626: assignment operator was implicitly defined as deleted
                #/wd4820 # C4820: 'bytes' bytes padding added after construct 'member_name'
                #/wd5027 # C5027: 'type': move assignment operator was implicitly defined as deleted
                #/wd5045 # C5045: Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
                #/wd5245 # C5245: 'function': unreferenced function with internal linkage has been removed

        )
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR
           CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

        # Clang and GCC
        target_compile_options(
            ${target}
            PRIVATE
                # Clang-CL specific
                # With clang-cl, don't enable -Wall because it will  map to -Weverything
                # Instead, enable -W4 witch map to -Wall and -Wextra
                $<$<AND:$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>,$<CXX_COMPILER_ID:Clang>>:-W4>
                # GCC & Clang (GNU) specific
                $<$<AND:$<CXX_COMPILER_FRONTEND_VARIANT:GNU>,$<CXX_COMPILER_ID:GNU,Clang>>:-Wall>
                $<$<AND:$<CXX_COMPILER_FRONTEND_VARIANT:GNU>,$<CXX_COMPILER_ID:GNU,Clang>>:-Wextra>
                -Wpedantic
                -Wno-comment             # this will trigger warning on doxygen comments which use latex matrix
                -Wnon-virtual-dtor        # warn the user if a class with virtual functions has a non-virtual destructor.
                #-Wshadow                 # warn the user if a variable declaration shadows one from a parent context
                #-Wold-style-cast         # warn for c-style casts
                #-Wcast-align             # warn for potential performance problem casts
                -Wunused                  # warn on anything being unused
                -Woverloaded-virtual     # warn if you overload (not override) a virtual function
                -Wconversion              # warn on type conversions that may lose data
                #-Wmisleading-indentation # warn if indentation implies blocks where blocks do not exist
                -Wnull-dereference        # warn if a null dereference is detected
                -Wdouble-promotion        # warn if float is implicit promoted to double
                -Wformat=2                # warn on security issues around functions that format output (ie printf)
                -Wimplicit-fallthrough    # warns when case statements fall-through.
                -Wswitch
                -Wswitch-default
                -Wswitch-enum
                -Wundef
                -Wextra-semi             # warn about redundant semicolons
                -Wsuggest-override       # warn about overriding virtual functions that are not marked with the override keyword.
        )

        if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            target_compile_options(
                ${target}
                PRIVATE
                    -Wuseless-cast        # warn if you perform a cast to the same type
                    -Wduplicated-cond     # warn if if / else chain has duplicated conditions
                    -Wduplicated-branches # warn if if / else branches have duplicated code
                    -Wlogical-op          # warn about suspicious uses of logical operators in expressions.
            )
        endif()

        if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            target_compile_options(
                ${target}
                PRIVATE
                    -Wextra-semi-stmt
                    -Wunreachable-code
                    -Wmissing-prototypes
                    -Wsuggest-destructor-override
                    -Winconsistent-missing-override            # is this necessary (Wsuggest-override is enable)
                    -Winconsistent-missing-destructor-override # is this necessary (Wsuggest-destructor-override is enable)
                    #-Wrange-loop-bind-reference
                    #-Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic
                    #-Wno-padded
                    #-Wno-unsafe-buffer-usage
                    #-Wno-documentation
                    #-Wno-shadow-field-in-constructor
                    #-Wno-float-equal
                    #-Wno-covered-switch-default
                    #-Wno-cast-function-type-strict
                    #-Wno-global-constructors # warn from GTest
                    #-Wno-shadow-uncaptured-local
                    #-Wno-weak-vtables
            )
        endif()

    endif()
endfunction(fuse_target_set_compiler_flags)
