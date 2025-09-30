# Endpoints

## Create Endpoint

Create an endpoint, currently available only for cloud endpoints

### Request

POST /endpoints

#### Example Request

```bash
curl \
-X POST \
-H "Authorization: Bearer {API_KEY}" \
-H "Content-Type: application/json" \
-H "Ngrok-Version: 2" \
-d '{"bindings":["public"],"description":"sample cloud endpoint","metadata":"{\"environment\": \"staging\"}","traffic_policy":"{\"on_http_request\":[{\"actions\":[{\"type\":\"deny\",\"config\":{\"status_code\":404}}]}]}","type":"cloud","url":"https://endpoint-example2.com:443"}' \
https://api.ngrok.com/endpoints
```

#### Parameters

| Name              | Type          | Description                                                                                                                |
| ----------------- | ------------- | -------------------------------------------------------------------------------------------------------------------------- |
| `url`             | string        | the url of the endpoint                                                                                                    |
| `type`            | string        | Type of endpoint. Only 'cloud' is currently supported (represents a cloud endpoint). Defaults to 'cloud' if not specified. |
| `traffic_policy`  | string        | The traffic policy attached to this endpoint                                                                               |
| `description`     | string        | user-supplied description of the associated tunnel                                                                         |
| `metadata`        | string        | user-supplied metadata of the associated tunnel or edge object                                                             |
| `bindings`        | List\<string> | the bindings associated with this endpoint                                                                                 |
| `pooling_enabled` | boolean       |                                                                                                                            |

### Response

Returns a 201 response on success

#### Example Response

```json
{
  "bindings": [
    "public"
  ],
  "created_at": "2025-09-04T10:11:35Z",
  "description": "sample cloud endpoint",
  "domain": {
    "id": "rd_32ELGgng2bHjKV4NCZ5GuWGEdtK",
    "uri": "https://api.ngrok.com/reserved_domains/rd_32ELGgng2bHjKV4NCZ5GuWGEdtK"
  },
  "hostport": "endpoint-example2.com:443",
  "id": "ep_32ELHJciGkvcKjnjhClgnkMIJSs",
  "metadata": "{\"environment\": \"staging\"}",
  "pooling_enabled": false,
  "proto": "https",
  "public_url": "https://endpoint-example2.com",
  "traffic_policy": "{\"on_http_request\":[{\"actions\":[{\"type\":\"deny\",\"config\":{\"status_code\":404}}]}]}",
  "type": "cloud",
  "updated_at": "2025-09-04T10:11:35Z",
  "uri": "https://api.ngrok.com/endpoints/ep_32ELHJciGkvcKjnjhClgnkMIJSs",
  "url": "https://endpoint-example2.com"
}
```

#### Fields

| Name                | Type               | Description                                                                                                                                               |
| ------------------- | ------------------ | --------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `id`                | string             | unique endpoint resource identifier                                                                                                                       |
| `region`            | string             | identifier of the region this endpoint belongs to                                                                                                         |
| `created_at`        | string             | timestamp when the endpoint was created in RFC 3339 format                                                                                                |
| `updated_at`        | string             | timestamp when the endpoint was updated in RFC 3339 format                                                                                                |
| `public_url`        | string             | URL of the hostport served by this endpoint                                                                                                               |
| `proto`             | string             | protocol served by this endpoint. one of `http`, `https`, `tcp`, or `tls`                                                                                 |
| `scheme`            | string             |                                                                                                                                                           |
| `hostport`          | string             | hostport served by this endpoint (hostname:port) -> soon to be deprecated                                                                                 |
| `host`              | string             |                                                                                                                                                           |
| `port`              | int64              |                                                                                                                                                           |
| `type`              | string             | whether the endpoint is `ephemeral` (served directly by an agent-initiated tunnel) or `edge` (served by an edge) or `cloud (represents a cloud endpoint)` |
| `metadata`          | string             | user-supplied metadata of the associated tunnel or edge object                                                                                            |
| `description`       | string             | user-supplied description of the associated tunnel                                                                                                        |
| `domain`            | [Ref](#ref-fields) | the domain reserved for this endpoint                                                                                                                     |
| `tcp_addr`          | [Ref](#ref-fields) | the address reserved for this endpoint                                                                                                                    |
| `tunnel`            | [Ref](#ref-fields) | the tunnel serving requests to this endpoint, if this is an ephemeral endpoint                                                                            |
| `edge`              | [Ref](#ref-fields) | the edge serving requests to this endpoint, if this is an edge endpoint                                                                                   |
| `upstream_url`      | string             | the local address the tunnel forwards to                                                                                                                  |
| `upstream_protocol` | string             | the protocol the agent uses to forward with                                                                                                               |
| `url`               | string             | the url of the endpoint                                                                                                                                   |
| `principal`         | [Ref](#ref-fields) | The ID of the owner (bot or user) that owns this endpoint                                                                                                 |
| `traffic_policy`    | string             | The traffic policy attached to this endpoint                                                                                                              |
| `bindings`          | List\<string>      | the bindings associated with this endpoint                                                                                                                |
| `tunnel_session`    | [Ref](#ref-fields) | The tunnel session of the agent for this endpoint                                                                                                         |
| `uri`               | string             | URI of the Cloud Endpoint API resource                                                                                                                    |
| `name`              | string             | user supplied name for the endpoint                                                                                                                       |
| `pooling_enabled`   | boolean            | whether the endpoint allows pooling                                                                                                                       |

#### Ref fields

| Name  | Type   | Description                   |
| ----- | ------ | ----------------------------- |
| `id`  | string | a resource identifier         |
| `uri` | string | a uri for locating a resource |

## List Endpoints

List all active endpoints on the account

### Request

GET /endpoints

#### Example Request

```bash
curl \
-X GET \
-H "Authorization: Bearer {API_KEY}" \
-H "Ngrok-Version: 2" \
https://api.ngrok.com/endpoints
```

### Response

Returns a 200 response on success

#### Example Response

```json
{
  "endpoints": [
    {
      "bindings": [
        "public"
      ],
      "created_at": "2025-09-04T10:11:35Z",
      "description": "sample cloud endpoint",
      "domain": {
        "id": "rd_32ELGgng2bHjKV4NCZ5GuWGEdtK",
        "uri": "https://api.ngrok.com/reserved_domains/rd_32ELGgng2bHjKV4NCZ5GuWGEdtK"
      },
      "hostport": "endpoint-example2.com:443",
      "id": "ep_32ELHJciGkvcKjnjhClgnkMIJSs",
      "metadata": "{\"environment\": \"staging\"}",
      "pooling_enabled": false,
      "proto": "https",
      "public_url": "https://endpoint-example2.com",
      "traffic_policy": "{\"on_http_request\":[{\"actions\":[{\"type\":\"deny\",\"config\":{\"status_code\":404}}]}]}",
      "type": "cloud",
      "updated_at": "2025-09-04T10:11:35Z",
      "uri": "https://api.ngrok.com/endpoints/ep_32ELHJciGkvcKjnjhClgnkMIJSs",
      "url": "https://endpoint-example2.com"
    },
    {
      "bindings": [
        "public"
      ],
      "created_at": "2025-09-04T10:11:33Z",
      "hostport": "e3983d8a80a8.ngrok.paid:443",
      "id": "ep_32ELH6ocn0487uK9sPzl8zBBTD8",
      "name": "command_line",
      "pooling_enabled": false,
      "principal": {
        "id": "usr_32ELAUA7XkJHRP2DgH2JjDRLp81",
        "uri": ""
      },
      "proto": "https",
      "public_url": "https://e3983d8a80a8.ngrok.paid",
      "tunnel": {
        "id": "tn_32ELH6ocn0487uK9sPzl8zBBTD8",
        "uri": "https://api.ngrok.com/tunnels/tn_32ELH6ocn0487uK9sPzl8zBBTD8"
      },
      "tunnel_session": {
        "id": "ts_32ELH0k19GLZadNsJvl8hhQua9k",
        "uri": "https://api.ngrok.com/tunnel_sessions/ts_32ELH0k19GLZadNsJvl8hhQua9k"
      },
      "type": "ephemeral",
      "updated_at": "2025-09-04T10:11:33Z",
      "upstream_url": "http://localhost:80",
      "url": "https://e3983d8a80a8.ngrok.paid"
    },
    {
      "bindings": [
        "public"
      ],
      "created_at": "2025-09-04T10:11:30Z",
      "domain": {
        "id": "rd_32ELGgng2bHjKV4NCZ5GuWGEdtK",
        "uri": "https://api.ngrok.com/reserved_domains/rd_32ELGgng2bHjKV4NCZ5GuWGEdtK"
      },
      "edge": {
        "id": "edgtls_32ELGjM9kI4eDTwg8Icxzwb16nN",
        "uri": "https://api.ngrok.com/edges/tls/edgtls_32ELGjM9kI4eDTwg8Icxzwb16nN"
      },
      "hostport": "endpoint-example2.com:443",
      "id": "ep_32ELGijftneZ2luJ1Ml2ZZuTkpA",
      "pooling_enabled": false,
      "proto": "tls",
      "public_url": "tls://endpoint-example2.com",
      "type": "edge",
      "updated_at": "2025-09-04T10:11:30Z"
    }
  ],
  "next_page_uri": null,
  "uri": "https://api.ngrok.com/endpoints"
}
```

#### Fields

| Name            | Type                         | Description                                            |
| --------------- | ---------------------------- | ------------------------------------------------------ |
| `endpoints`     | [Endpoint](#endpoint-fields) | the list of all active endpoints on this account       |
| `uri`           | string                       | URI of the endpoints list API resource                 |
| `next_page_uri` | string                       | URI of the next page, or null if there is no next page |

#### Endpoint fields

| Name                | Type               | Description                                                                                                                                               |
| ------------------- | ------------------ | --------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `id`                | string             | unique endpoint resource identifier                                                                                                                       |
| `region`            | string             | identifier of the region this endpoint belongs to                                                                                                         |
| `created_at`        | string             | timestamp when the endpoint was created in RFC 3339 format                                                                                                |
| `updated_at`        | string             | timestamp when the endpoint was updated in RFC 3339 format                                                                                                |
| `public_url`        | string             | URL of the hostport served by this endpoint                                                                                                               |
| `proto`             | string             | protocol served by this endpoint. one of `http`, `https`, `tcp`, or `tls`                                                                                 |
| `scheme`            | string             |                                                                                                                                                           |
| `hostport`          | string             | hostport served by this endpoint (hostname:port) -> soon to be deprecated                                                                                 |
| `host`              | string             |                                                                                                                                                           |
| `port`              | int64              |                                                                                                                                                           |
| `type`              | string             | whether the endpoint is `ephemeral` (served directly by an agent-initiated tunnel) or `edge` (served by an edge) or `cloud (represents a cloud endpoint)` |
| `metadata`          | string             | user-supplied metadata of the associated tunnel or edge object                                                                                            |
| `description`       | string             | user-supplied description of the associated tunnel                                                                                                        |
| `domain`            | [Ref](#ref-fields) | the domain reserved for this endpoint                                                                                                                     |
| `tcp_addr`          | [Ref](#ref-fields) | the address reserved for this endpoint                                                                                                                    |
| `tunnel`            | [Ref](#ref-fields) | the tunnel serving requests to this endpoint, if this is an ephemeral endpoint                                                                            |
| `edge`              | [Ref](#ref-fields) | the edge serving requests to this endpoint, if this is an edge endpoint                                                                                   |
| `upstream_url`      | string             | the local address the tunnel forwards to                                                                                                                  |
| `upstream_protocol` | string             | the protocol the agent uses to forward with                                                                                                               |
| `url`               | string             | the url of the endpoint                                                                                                                                   |
| `principal`         | [Ref](#ref-fields) | The ID of the owner (bot or user) that owns this endpoint                                                                                                 |
| `traffic_policy`    | string             | The traffic policy attached to this endpoint                                                                                                              |
| `bindings`          | List\<string>      | the bindings associated with this endpoint                                                                                                                |
| `tunnel_session`    | [Ref](#ref-fields) | The tunnel session of the agent for this endpoint                                                                                                         |
| `uri`               | string             | URI of the Cloud Endpoint API resource                                                                                                                    |
| `name`              | string             | user supplied name for the endpoint                                                                                                                       |
| `pooling_enabled`   | boolean            | whether the endpoint allows pooling                                                                                                                       |

#### Ref fields

| Name  | Type   | Description                   |
| ----- | ------ | ----------------------------- |
| `id`  | string | a resource identifier         |
| `uri` | string | a uri for locating a resource |

## Get Endpoint

Get the status of an endpoint by ID

### Request

GET /endpoints/\{id}

#### Example Request

```bash
curl \
-X GET \
-H "Authorization: Bearer {API_KEY}" \
-H "Ngrok-Version: 2" \
https://api.ngrok.com/endpoints/ep_32ELHJciGkvcKjnjhClgnkMIJSs
```

### Response

Returns a 200 response on success

#### Example Response

```json
{
  "bindings": [
    "public"
  ],
  "created_at": "2025-09-04T10:11:35Z",
  "description": "sample cloud endpoint",
  "domain": {
    "id": "rd_32ELGgng2bHjKV4NCZ5GuWGEdtK",
    "uri": "https://api.ngrok.com/reserved_domains/rd_32ELGgng2bHjKV4NCZ5GuWGEdtK"
  },
  "hostport": "endpoint-example2.com:443",
  "id": "ep_32ELHJciGkvcKjnjhClgnkMIJSs",
  "metadata": "{\"environment\": \"staging\"}",
  "pooling_enabled": false,
  "proto": "https",
  "public_url": "https://endpoint-example2.com",
  "traffic_policy": "{\"on_http_request\":[{\"actions\":[{\"type\":\"deny\",\"config\":{\"status_code\":404}}]}]}",
  "type": "cloud",
  "updated_at": "2025-09-04T10:11:35Z",
  "uri": "https://api.ngrok.com/endpoints/ep_32ELHJciGkvcKjnjhClgnkMIJSs",
  "url": "https://endpoint-example2.com"
}
```

#### Fields

| Name                | Type               | Description                                                                                                                                               |
| ------------------- | ------------------ | --------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `id`                | string             | unique endpoint resource identifier                                                                                                                       |
| `region`            | string             | identifier of the region this endpoint belongs to                                                                                                         |
| `created_at`        | string             | timestamp when the endpoint was created in RFC 3339 format                                                                                                |
| `updated_at`        | string             | timestamp when the endpoint was updated in RFC 3339 format                                                                                                |
| `public_url`        | string             | URL of the hostport served by this endpoint                                                                                                               |
| `proto`             | string             | protocol served by this endpoint. one of `http`, `https`, `tcp`, or `tls`                                                                                 |
| `scheme`            | string             |                                                                                                                                                           |
| `hostport`          | string             | hostport served by this endpoint (hostname:port) -> soon to be deprecated                                                                                 |
| `host`              | string             |                                                                                                                                                           |
| `port`              | int64              |                                                                                                                                                           |
| `type`              | string             | whether the endpoint is `ephemeral` (served directly by an agent-initiated tunnel) or `edge` (served by an edge) or `cloud (represents a cloud endpoint)` |
| `metadata`          | string             | user-supplied metadata of the associated tunnel or edge object                                                                                            |
| `description`       | string             | user-supplied description of the associated tunnel                                                                                                        |
| `domain`            | [Ref](#ref-fields) | the domain reserved for this endpoint                                                                                                                     |
| `tcp_addr`          | [Ref](#ref-fields) | the address reserved for this endpoint                                                                                                                    |
| `tunnel`            | [Ref](#ref-fields) | the tunnel serving requests to this endpoint, if this is an ephemeral endpoint                                                                            |
| `edge`              | [Ref](#ref-fields) | the edge serving requests to this endpoint, if this is an edge endpoint                                                                                   |
| `upstream_url`      | string             | the local address the tunnel forwards to                                                                                                                  |
| `upstream_protocol` | string             | the protocol the agent uses to forward with                                                                                                               |
| `url`               | string             | the url of the endpoint                                                                                                                                   |
| `principal`         | [Ref](#ref-fields) | The ID of the owner (bot or user) that owns this endpoint                                                                                                 |
| `traffic_policy`    | string             | The traffic policy attached to this endpoint                                                                                                              |
| `bindings`          | List\<string>      | the bindings associated with this endpoint                                                                                                                |
| `tunnel_session`    | [Ref](#ref-fields) | The tunnel session of the agent for this endpoint                                                                                                         |
| `uri`               | string             | URI of the Cloud Endpoint API resource                                                                                                                    |
| `name`              | string             | user supplied name for the endpoint                                                                                                                       |
| `pooling_enabled`   | boolean            | whether the endpoint allows pooling                                                                                                                       |

#### Ref fields

| Name  | Type   | Description                   |
| ----- | ------ | ----------------------------- |
| `id`  | string | a resource identifier         |
| `uri` | string | a uri for locating a resource |

## Update Endpoint

Update an Endpoint by ID, currently available only for cloud endpoints

### Request

PATCH /endpoints/\{id}

#### Example Request

```bash
curl \
-X PATCH \
-H "Authorization: Bearer {API_KEY}" \
-H "Content-Type: application/json" \
-H "Ngrok-Version: 2" \
-d '{"description":"Sample Cloud Endpoint"}' \
https://api.ngrok.com/endpoints/ep_32ELHJciGkvcKjnjhClgnkMIJSs
```

#### Parameters

| Name              | Type          | Description                                                    |
| ----------------- | ------------- | -------------------------------------------------------------- |
| `id`              | string        | unique endpoint resource identifier                            |
| `url`             | string        | the url of the endpoint                                        |
| `traffic_policy`  | string        | The traffic policy attached to this endpoint                   |
| `description`     | string        | user-supplied description of the associated tunnel             |
| `metadata`        | string        | user-supplied metadata of the associated tunnel or edge object |
| `bindings`        | List\<string> | the bindings associated with this endpoint                     |
| `pooling_enabled` | boolean       |                                                                |

### Response

Returns a 200 response on success

#### Example Response

```json
{
  "bindings": [
    "public"
  ],
  "created_at": "2025-09-04T10:11:35Z",
  "description": "Sample Cloud Endpoint",
  "domain": {
    "id": "rd_32ELGgng2bHjKV4NCZ5GuWGEdtK",
    "uri": "https://api.ngrok.com/reserved_domains/rd_32ELGgng2bHjKV4NCZ5GuWGEdtK"
  },
  "hostport": "endpoint-example2.com:443",
  "id": "ep_32ELHJciGkvcKjnjhClgnkMIJSs",
  "metadata": "{\"environment\": \"staging\"}",
  "pooling_enabled": false,
  "proto": "https",
  "public_url": "https://endpoint-example2.com",
  "traffic_policy": "{\"on_http_request\":[{\"actions\":[{\"type\":\"deny\",\"config\":{\"status_code\":404}}]}]}",
  "type": "cloud",
  "updated_at": "2025-09-04T10:11:35Z",
  "uri": "https://api.ngrok.com/endpoints/ep_32ELHJciGkvcKjnjhClgnkMIJSs",
  "url": "https://endpoint-example2.com"
}
```

#### Fields

| Name                | Type               | Description                                                                                                                                               |
| ------------------- | ------------------ | --------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `id`                | string             | unique endpoint resource identifier                                                                                                                       |
| `region`            | string             | identifier of the region this endpoint belongs to                                                                                                         |
| `created_at`        | string             | timestamp when the endpoint was created in RFC 3339 format                                                                                                |
| `updated_at`        | string             | timestamp when the endpoint was updated in RFC 3339 format                                                                                                |
| `public_url`        | string             | URL of the hostport served by this endpoint                                                                                                               |
| `proto`             | string             | protocol served by this endpoint. one of `http`, `https`, `tcp`, or `tls`                                                                                 |
| `scheme`            | string             |                                                                                                                                                           |
| `hostport`          | string             | hostport served by this endpoint (hostname:port) -> soon to be deprecated                                                                                 |
| `host`              | string             |                                                                                                                                                           |
| `port`              | int64              |                                                                                                                                                           |
| `type`              | string             | whether the endpoint is `ephemeral` (served directly by an agent-initiated tunnel) or `edge` (served by an edge) or `cloud (represents a cloud endpoint)` |
| `metadata`          | string             | user-supplied metadata of the associated tunnel or edge object                                                                                            |
| `description`       | string             | user-supplied description of the associated tunnel                                                                                                        |
| `domain`            | [Ref](#ref-fields) | the domain reserved for this endpoint                                                                                                                     |
| `tcp_addr`          | [Ref](#ref-fields) | the address reserved for this endpoint                                                                                                                    |
| `tunnel`            | [Ref](#ref-fields) | the tunnel serving requests to this endpoint, if this is an ephemeral endpoint                                                                            |
| `edge`              | [Ref](#ref-fields) | the edge serving requests to this endpoint, if this is an edge endpoint                                                                                   |
| `upstream_url`      | string             | the local address the tunnel forwards to                                                                                                                  |
| `upstream_protocol` | string             | the protocol the agent uses to forward with                                                                                                               |
| `url`               | string             | the url of the endpoint                                                                                                                                   |
| `principal`         | [Ref](#ref-fields) | The ID of the owner (bot or user) that owns this endpoint                                                                                                 |
| `traffic_policy`    | string             | The traffic policy attached to this endpoint                                                                                                              |
| `bindings`          | List\<string>      | the bindings associated with this endpoint                                                                                                                |
| `tunnel_session`    | [Ref](#ref-fields) | The tunnel session of the agent for this endpoint                                                                                                         |
| `uri`               | string             | URI of the Cloud Endpoint API resource                                                                                                                    |
| `name`              | string             | user supplied name for the endpoint                                                                                                                       |
| `pooling_enabled`   | boolean            | whether the endpoint allows pooling                                                                                                                       |

#### Ref fields

| Name  | Type   | Description                   |
| ----- | ------ | ----------------------------- |
| `id`  | string | a resource identifier         |
| `uri` | string | a uri for locating a resource |

## Delete Endpoint

Delete an Endpoint by ID, currently available only for cloud endpoints

### Request

DELETE /endpoints/\{id}

#### Example Request

```bash
curl \
-X DELETE \
-H "Authorization: Bearer {API_KEY}" \
-H "Ngrok-Version: 2" \
https://api.ngrok.com/endpoints/ep_32ELHJciGkvcKjnjhClgnkMIJSs
```

### Response

Returns a 204 response with no body on success
