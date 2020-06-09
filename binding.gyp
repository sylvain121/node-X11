{
  "targets": [
    {
      "target_name": "node_x11",
      "sources": [
        "src/cpp/export.cc",
        "src/cpp/xdisplay.c"
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ],
      "link_settings": {
        "libraries": [
          "-lXtst",
          "-lX11",
          "-lXfixes",
          "-lXrandr"
        ]
      }
    }
  ]
}
