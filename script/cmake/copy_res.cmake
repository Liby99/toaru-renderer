# Macro to generate copy_${type} functions
macro(add_copy_res_function type prefix)
  function("copy_${type}" name)

    # TRICK: use ${${func_argn}} to save the function argn from the macro
    set(func_argn ARGN)
    set(func_options FROM_CURRENT_RES)
    set(func_one_arg_options FROM TO AS)
    cmake_parse_arguments(COPY_RES "${func_options}" "${func_one_arg_options}" "" ${${func_argn}})

    # Set from directory
    if(COPY_RES_FROM_CURRENT_RES)
      set(from_dir "${source_dir}/res/${prefix}/")
    elseif(COPY_RES_FROM)
      set(from_dir "${COPY_RES_FROM}/")
    else()
      set(from_dir "${curr_dir}/res/${prefix}/")
    endif()

    # Set destination directory
    set(dest_dir "${target_dir}/${prefix}/")
    if(COPY_RES_TO)
      set(dest_dir "${dest_dir}/${COPY_RES_TO}/")
    endif()

    # Copy the file from FROM_DIR to DEST_DIR
    if(NOT "${ARGN}" STREQUAL "")

      # If contains extensions in ARGN, then copy the file with extensions
      foreach(extension ${ARGN})
        file(COPY "${from_dir}${name}${extension}" DESTINATION "${dest_dir}")
      endforeach()
    else()

      # If not contains extension, then directly copy the file with the name
      file(COPY "${from_dir}${name}" DESTINATION "${dest_dir}")

      # If has AS option, then rename the file.
      # Note that only when copying one single file one can do the renaming
      if(COPY_RES_AS)
        get_filename_component(temp_name "${name}" NAME)
        file(RENAME "${build_dir}/${dest_dir}${temp_name}" "${build_dir}/${dest_dir}${COPY_RES_AS}")
      endif()
    endif()
  endfunction()
endmacro()

# Function to copy shader. Will copy vert and frag shaders simultaneously
add_copy_res_function(shader "shader" ".vert.glsl" ".frag.glsl")

# Functions to copy images. When copying cubemap, one should be specifying only the directory
add_copy_res_function(image "image")
add_copy_res_function(cubemap "image/cubemap" "/posx.jpg" "/negx.jpg" "/posy.jpg" "/negy.jpg" "/posz.jpg" "/negz.jpg")

# Functions to copy model, coef and scene
add_copy_res_function(model "model")
add_copy_res_function(coefs "coefs")
add_copy_res_function(scene "scene")