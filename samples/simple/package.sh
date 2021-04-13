#!/usr/bin/env bash
set -e

declare comment_doc=$(
    cat <<'EOFe9576f8668e0' | grep -v '#.*org-mode%'
# {%org-mode%}
package files for arm64
# {%/org-mode%}
EOFe9576f8668e0
        )

## start code-generator "^\\s *#\\s *"
# generate-getopt -P '?"${comment_doc}"' \
    # f:files="()" '?"打包哪些文件"' \
    # d:arch-dir=/usr/aarch64-linux-gnu/ '?"arch 对应的目录，默认是 arm64 的 /usr/aarch64-linux-gnu"' \
    # o:output '?"最终输出保存的文件"' \

## end code-generator
## start generated code
TEMP=$(POSIXLY_CORRECT=true getopt -o d:f:o:h \
                      --long arch-dir:,files:,output:,help \
                      -n "$(basename -- "$0")" -- "$@")
declare arch_dir=/usr/aarch64-linux-gnu/
declare -a files=()
declare output=
eval set -- "$TEMP"
while true; do
    case "$1" in

        -d|--arch-dir)
            arch_dir=$2
            shift 2

            ;;
        -f|--files)
            files=("${files[@]}" "$2")
            shift 2

            ;;
        -o|--output)
            output=$2
            shift 2

            ;;
        -h|--help)
            set +x
            echo -e "${comment_doc}"
            echo
            echo Options and arguments:
            printf %6s '-d, '
            printf %-24s '--arch-dir=ARCH_DIR'
            echo "arch 对应的目录，默认是 arm64 的 /usr/aarch64-linux-gnu"
            printf %6s '-f, '
            printf %-24s '--files=FILES...'
            echo "打包哪些文件"
            printf %6s '-o, '
            printf %-24s '--output=OUTPUT'
            echo "最终输出保存的文件"
            exit
            shift
            ;;
        --)
            shift
            break
            ;;
        *)
            die "internal error: $(. bt; echo; bt | indent-stdin)"
            ;;
    esac
done


## end generated code

tar czfv "${output}" "${arch_dir}" "${files[@]}"
