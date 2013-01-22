#!/bin/sh

OUTPUT_FILE=bear-factory.pot

if ! which xpath >/dev/null
then
  echo "Can't find xpath command in your paths."
  exit 1
fi

# Output the gettext string of an xml <description> node
# $1 is the XPath of the node to extract
# $2 is the file in which the node is extracted
# $3 is the comment printed before the key string
mkstring()
{
    xpath -q -e "$1" "$2" 2>/dev/null \
	| tr '\t\n' ' ' \
        | sed 's:</description>:\n:g' \
        | sed 's:<description>::' \
	| tr -s ' ' \
        | sed 's/^ \+//;s/ \+$//;s/"/\\\\"/g' \
        | while read L
          do
            echo "# $3"
            echo "# $2"
            echo "msgid \"$L\""
            echo 'msgstr ""'
            echo
          done
}

(
    xgettext --output=- --c++ --keyword=_ --trigraphs --sort-output \
	$(find ../../../ -name "*.[tch]pp") \
      | sed 's/charset=CHARSET/charset=UTF-8/'

    find ../../../item-description/generic/ -name "*.xml" \
	| while read F
          do
	    mkstring "/item/description" $F "Class description"
	    mkstring "/item/fields/field/description" $F "Field description"
          done
) | msguniq | msgcat - --sort-output > "$OUTPUT_FILE"

for f in *.po
do
    msgmerge --update --previous --backup=none "$f" "$OUTPUT_FILE"
done
