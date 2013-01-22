file(GLOB BF_IMAGES "*.png")

install(
  FILES ${BF_IMAGES}
  DESTINATION "${BF_IMAGES_PATH}"
)

