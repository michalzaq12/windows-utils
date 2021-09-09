{
  "targets": [
    {
      "target_name": "visualEffects",
	  "include_dirs" : [
        "src",
        "<!(node -e \"require('nan')\")"
	  ],
      "sources": [ "src/visualEffects.cc" ]
    }
  ]
}