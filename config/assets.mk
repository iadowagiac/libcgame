.PHONY: clean-assets assets

# Get a list of entity source files.
AWKCMD1 = '$$3!=""{print "assets/entities/"$$3}'

# Get a list of scene source files.
AWKCMD2 = '$$3!=""{print "assets/scenes/"$$3}'

# Generate an enum of entity tags for assets.h
AWKCMD3 = 'BEGIN{print "enum\\n{\\n    TAG_NONE,"}' \
          '$$1!=""{print "    "$$1","}' \
          'END{print "};"}'

# Generate an enum of entity types for assets.h
AWKCMD4 = 'BEGIN{print "enum\\n{\\n    EN_NONE,"}' \
          '$$1!=""{print "    "$$1","}' \
          'END{print "};"}'

# Generate an enum of scenes for assets.h
AWKCMD5 = 'BEGIN{print "enum\\n{\\n    SC_NONE,"}' \
          '$$1!=""{print "    "$$1","}' \
          'END{print "};"}'

# Generate the list of entity table imports for the assets.c file.
AWKCMD6 = '$$2!=""{print "extern const struct entity_type " $$2";"}'

# Generate the list of scene imports for the assets.c file.
AWKCMD7 = '$$2!=""{print "extern const struct scene " $$2";"}'

# Generate an entity table for the assets.c file.
AWKCMD8 = 'BEGIN{print "const struct entity_type *entity_types[] = \\n{\\n    NULL,"}' \
          '$$2!=""{print "    &"$$2","; ++entity_count}' \
          'END{print "};"}' \
          'END{print "\\nint entity_type_count = " (entity_count + 1) ";"}'

# Generate an scene table for the assets.c file.
AWKCMD9 = 'BEGIN{print "const struct scene *scenes[] = \\n{\\n    NULL,"}' \
          '$$2!=""{print "    &"$$2","; ++scene_count}' \
          'END{print "};"}' \
          'END{print "\\nint scene_count = " (scene_count + 1) ";"}'

# Object Files for Assets.
ASSETOBJS = ${ENTITYOBJS} ${SCENEOBJS} assets/assets.o

ENTITYOBJS = ${ENTITYSRCS:.c=.o}
ENTITYSRCS = ${shell awk ${AWKCMD1} assets/entities.lst}

SCENEOBJS = ${SCENESRCS:.c=.o}
SCENESRCS = ${shell awk ${AWKCMD2} assets/scenes.lst}

EXTRACLEAN += clean-assets

default: all

clean-assets:
	rm -f assets/assets.c assets/assets.h ${ASSETOBJS}

assets: ${ASSETOBJS}

${ENTITYOBJS}: assets/assets.h

${SCENEOBJS}: assets/assets.h

assets/assets.c: assets/assets.h ${ENTITYSRCS}
	echo -n "#include <stdlib.h>\n#include \"cgame.h\"\n\n" > $@
	awk ${shell echo "'" ${AWKCMD6} "'"} assets/entities.lst >> $@
	echo >> $@
	awk ${shell echo "'" ${AWKCMD7} "'"} assets/scenes.lst >> $@
	echo >> $@
	awk ${shell echo "'" ${AWKCMD8} "'"} assets/entities.lst >> $@
	echo >> $@
	awk ${shell echo "'" ${AWKCMD9} "'"} assets/scenes.lst >> $@

assets/assets.h: assets/tags.lst assets/entities.lst
	echo -n "#ifndef ASSETS_H\n#define ASSETS_H\n\n" > $@
	awk ${shell echo "'" ${AWKCMD3} "'"} assets/tags.lst >> $@
	echo >> $@
	awk ${shell echo "'" ${AWKCMD4} "'"} assets/entities.lst >> $@
	echo >> $@
	awk ${shell echo "'" ${AWKCMD5} "'"} assets/scenes.lst >> $@
	echo -n "\n#endif // ndef ASSETS_H" >> $@

