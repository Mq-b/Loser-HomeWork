#!/bin/bash

function show_help_info {
	echo "Usage: $0 [options]

Options:
  -e, --execution execution  Run target execution (mandatory)
  -o, --output-name output  Output file name for run (mandatory)
  -c, --correct-output  Correct output for run (mandatory)
  -i, --input  Input for run
  -h, --help              Show this help info"
}

function fail {
	printf '%s\n' "$1" >&2
	exit "${2-1}"
}

function get_opt_and_optarg {
	local optional=false

	if [[ $1 == @optional ]]; then
		optional=true
		shift
	fi

	OPT=$1 OPTARG= OPTSHIFT=0

	if [[ $1 == -[!-]?* ]]; then
		OPT=${1:0:2} OPTARG=${1:2}
	elif [[ $1 == --*=* ]]; then
		OPT=${1%%=*} OPTARG=${1#*=}
	elif [[ ${2+.} && (${optional} == false || $2 != -?*) ]]; then
		OPTARG=$2 OPTSHIFT=1
	elif [[ ${optional} == true ]]; then
		return 1
	else
		fail "No argument specified for '$1'."
	fi

	return 0
}

function main {
	local execution=() output_name=() input=() correct_output=()

	if [[ $# -eq 0 ]]; then
    show_help_info
    return 2
	fi

	while [[ $# -gt 0 ]]; do
		case $1 in
		-h|--help)
			show_help_info
			return 2
			;;
		-e|--execution)
      get_opt_and_optarg "${@:1:2}"
      execution=${OPTARG}
      shift "${OPTSHIFT}"
			;;
		-o|--output-name)
      get_opt_and_optarg "${@:1:2}"
      output_name=${OPTARG}
      shift "${OPTSHIFT}"
			;;
		-i|--input)
      get_opt_and_optarg "${@:1:2}"
      input=${OPTARG}
      shift "${OPTSHIFT}"
      ;;
		-c|--correct-output)
      get_opt_and_optarg "${@:1:2}"
      correct_output=${OPTARG}
      shift "${OPTSHIFT}"
			;;
		-?*)
			fail "Invalid option: $1"
			;;
		esac

		shift
	done

  if [ -z "$execution" ] || [ -z "$output_name" ] || [ -z "$correct_output" ]; then
    show_help_info
    return 2
  fi

  output_file="$output_name.txt"
	if [ -f "$input" ]; then
      $execution < "$input" > "$output_file"
  else
      $execution > "$output_file"
  fi

  comparison=$(diff -q --strip-trailing-cr "$correct_output" "$output_file")
  if [ "$comparison" == "" ]; then
      echo "check result: same"
  else
      echo "check result: diff"
      echo "Current Output:"
      cat "$output_file" && echo ""
      echo "Correct Output:"
      cat "$correct_output" && echo ""
      exit 1
  fi
}

main "$@"