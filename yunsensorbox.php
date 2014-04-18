<?php

define('ETHERNET_IP', "127.0.0.1"); // replace with the Ethernet IP of your Yun
define('WIFI_IP', "127.0.0.1"); // replace with the Wifi IP of your Yun. If no wifi, repeat Ethernet IP

$endpoint = "http://" . ETHERNET_IP . "/data/get/";

@$dataJson = file_get_contents($endpoint);


if(strlen($dataJson) == 0) {
        // fall back to wifi
        $endpoint = "http://" . WIFI_IP . "/data/get/";
        @$dataJson = file_get_contents($endpoint);
}

$data = json_decode($dataJson);

if(isset($data->value)) {
        $realData = $data->value;
} else {
        $realData = array('status' => 'notresponding');
}

echo json_encode($realData);
