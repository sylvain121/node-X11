{
  "targets": [
  {
    "target_name": "node_x11",
      "sources": [ "export.cc", "xdisplay.c"  ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"

      ]

  }

  ]

}