# COCO Dataset Reader in C++

Reading a COCO dataset JSON file in C++ (and preview it with Qt)

Why -- Because I needed it and wanted to publish it.

Qt Dependency -- Makes it easier to visualize.

Useful to preview dataset exported from CVAT in COCO format and to generate masks if needed.
Masks can also be generated from bounding boxes for more less fine-grained masks.

Thankfully built with [json](https://github.com/nlohmann/json)

## Expected structure
This format is created by [CVAT](https://github.com/cvat-ai/cvat) when exporting to COCO.

```
"annotations" :[
        {
            "area": 1595.0,
            "attributes": {
                "occluded": false
            },
            "bbox": [
                2196,
                277,
                47,
                44
            ],
            "category_id": 1,
            "id": 1,
            "image_id": 1,
            "iscrowd": 0,
            "segmentation": [
                [
                    2216,
                    320,
                    2224,
                    321,
                    2235,
                    317,
                    2241,
                    308,
                    2243,
                    298,
                    2241,
                    288,
                    2235,
                    279,
                    2223,
                    277,
                    2216,
                    278,
                    2209,
                    281,
                    2201,
                    285,
                    2198,
                    292,
                    2196,
                    299,
                    2198,
                    306,
                    2202,
                    314,
                    2209,
                    317,
                    2216,
                    320
                ]
            ]
        }, ...
    ],
    "images": [
        {
            "coco_url": "",
            "date_captured": 0,
            "file_name": "imagename.png",
            "flickr_url": "",
            "height": 1920,
            "id": 1,
            "license": 0,
            "width": 2560
        }, ...
    ],
    "categories": [
        {
            "id": 1,
            "name": "object",
            "supercategory": ""
        }, ...
    ],
    "info": {
        "contributor": "",
        "date_created": "1976-04-01",
        "description": "",
        "url": "",
        "version": "",
        "year": 2025
    },
    "licenses": [
        {
            "id": 0,
            "name": "",
            "url": ""
        }, ...
    ]
```

## OPL

- [ ] Category filter
- [ ] Non-binary mask

Ideas ? -> Please create an issue

## License

LGPLv3 