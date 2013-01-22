#!/bin/sh

OUTPUT_FILE=bear-engine.pot

if ! which xpath >/dev/null
then
  echo "Can't find xpath command in your paths."
  exit 1
fi

# $1 is the XPath
# $2 is the XML file
xpath_wrapper()
{
    echo '<node/>' | xpath -e "/node" >/dev/null 2>/dev/null

    if [ $? -eq 0 ]
    then
	xpath -q -e "$1" "$2"
    else
	xpath "$2" "$1"
    fi
}

(
    echo "Searching strings in source code." 1>&2
    xgettext --output=- --c++ --trigraphs --sort-output --keyword= \
	--keyword=bear_gettext \
	$(find ../../../ -name "*.[tch]pp") \
      | sed 's/charset=CHARSET/charset=UTF-8/'

    echo "Searching strings Bear Engine's input module." 1>&2
    grep 'return "[[:lower:]].\+"' \
	../../../core/src/input/code/mouse.cpp \
	../../../core/src/input/code/keyboard.cpp \
	| cut -d'"' -f2 \
	| sed 's/^/msgid "/;s/$/"\nmsgstr ""\n/'

    grep 'result *= "[[:lower:]][[:lower:] ]\+"' \
	../../../core/src/input/code/joystick.cpp \
	| cut -d'"' -f2 \
	| sed 's/^/msgid "/;s/$/"\nmsgstr ""\n/'

) | iconv --from=iso-8859-15 --to=utf-8 \
    | msguniq | msgcat - --sort-output > "$OUTPUT_FILE"

for f in *.po
do
    msgmerge --update --previous --backup=none "$f" "$OUTPUT_FILE"
done
