nebula_begin_app(nebula-demo windowed)

fips_dir(.)
    add_blueprint(
        data/tables/demo_blueprints.json
    )

    add_template_dir(
        data/tables/templates
    )
    
    fips_files(
        main.cc
        demogamefeatureunit.h
        demogamefeatureunit.cc
        gamestatemanager.h
        gamestatemanager.cc
    )

    fips_dir(managers)
        fips_files(
            playermanager.h
            playermanager.cc
            inputmanager.h
            inputmanager.cc
            movementmanager.h
            movementmanager.cc
        )

    fips_dir(properties)
        nebula_add_nidl(
            input.nidl
            movement.nidl
        )

fips_deps(foundation application render resource dynui scripting editor)
nebula_end_app()
target_include_directories(nebula-demo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})