{
    "targets": [{
            "target_name": "node_x11",
            "sources": ["export.cc", "xdisplay.c"],
            "include_dirs": [
                "<!(node -e \"require('nan')\")"

            ],
            "cflags": [
                "-Wall",
                "-O3"

            ],
            "link_settings": {
                "libraries": [
                    "-lXtst",
                    "-lX11"
                ]

            }

        }

    ]

}
