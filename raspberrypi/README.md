A simple REST API which runs as a `systemd` service  is used to integrate with the home automation system. I use [Home Assistant](https://www.home-assistant.io/) and a REST API can be integrated like this (`configuration.yaml`)

```
rest_command:
  toggle_garage_door:
    url: 'http://<the-host>:<the-port>/garage/trigger'
    method: 'post'

script:
  toggle_garage_door:
    sequence:
      - service: rest_command.toggle_garage_door
```

## Prerequisites

Requires `virtualenv`

## Install dependencies

```
mkvirtualenv relay-garage-controller
pip install -r requirements.txt
```

## Start application

```
gunicorn -w 2 -b 0.0.0.0:5000 garage_controller:app
```

## Install as linux service

```
./scripts/build.sh && sudo ./scripts/install.sh
```

## Control garage door

`curl -X POST http://localhost:5000/garage/trigger`
