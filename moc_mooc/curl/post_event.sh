#!/usr/bin/env bash

curl --verbose  --connect-timeout 5 -X POST -H "Content-Type: application/json" -k --user myuser:mypass -d @postData.json https://97.76.231.101/events/new


