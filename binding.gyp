{
  "targets": [
    {
      "target_name": "visualEffects",
	    "include_dirs" : [
        "src",
        "<!(node -e \"require('nan')\")"
	    ],
      "sources": [ ],
      "conditions": [
        ['OS=="win"', {
          "sources": ["src/visualEffects.cc"],
        }]
      ],
    }
  ]
}