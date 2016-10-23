{
  'targets': [
    {
      "target_name": "node_dht_sensor",
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ],
      "libraries": [ "-lmraa" ],
      "conditions": [ 
        ["OS=='linux'", {
          "sources": ["node-dht-sensor.cpp","dht22.c"] 
        }]
      ]
    }
  ]
}
