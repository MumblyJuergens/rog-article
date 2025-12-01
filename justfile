default:
    @just --list

copyres TYPE='Debug':
    mkdir -p build/{{TYPE}}/res
    cp res/*.png build/{{TYPE}}/res

configure TYPE='Debug' CXX='g++' CC='gcc':
    cmake . -B build/{{TYPE}} -DCMAKE_BUILD_TYPE={{TYPE}} -DCMAKE_CXX_COMPILER={{CXX}} -DCMAKE_C_COMPILER={{CC}}

build TYPE='Debug' CXX='g++' CC='gcc': (configure TYPE CXX CC)
    cmake --build build/{{TYPE}} --config {{TYPE}}

run TYPE='Debug' CXX='g++' CC='gcc': (build TYPE CXX CC) (copyres TYPE)
    build/{{TYPE}}/rog_article

fpstest TYPE='Debug':
    build/{{TYPE}}/rog_article fpstest

[working-directory: 'build/Debug']
profile:
    valgrind --tool=cachegrind ./rog_article
    ls -Atr | tail -n 1 | xargs -I{} cg_annotate {} | less

[working-directory: 'build/Release']
profile-release:
    valgrind --tool=cachegrind ./rog_article
    ls -Atr | tail -n 1 | xargs -I{} cg_annotate {} | less


[working-directory: 'build/Debug']
perf-stat:
    perf stat -B -e cache-misses,cache-references ./rog_article

[working-directory: 'build/Release']
perf-stat-release:
    perf stat -B -e cache-misses,cache-references ./rog_article

[working-directory: 'build/Debug']
perf-report:
    perf record -B -e cache-misses,cache-references ./rog_article
    perf report -Mintel 

clean TYPE='Debug':
    cmake --build build/{{TYPE}} --target clean

