#include "app_wireless.h"
#include "WiFi.h"
#include "global_flags.h"
#include "ui.h"
// #include <NimBLE/NimBLEDevice.h>
// #include <string.h>

static lv_obj_t *ta2 = NULL;

static const char * btnm_map0[] = {"1", "2", "3", "\n",
                            "4", "5", "6", "\n",
                            "7", "8", "9", "\n",
                            LV_SYMBOL_BACKSPACE, "0", LV_SYMBOL_NEW_LINE, ""};

static const char * btnm_map1[] = {"Q", "W", "E", "\n",
                            "R", "T", "Y", "\n",
                            "U", "I", "O", "\n",
                            "P", "A", "S", ""};

static const char * btnm_map2[] = {"D", "F", "G", "\n",
                            "H", "J", "K", "\n",
                            "L", "H", "I", "\n",
                            "Z", "X", "C", ""};

static const char * btnm_map3[] = {"V", "B", "N", "\n",
                            "M", ",", ".", "\n",
                            "/", ";", "'", "\n",
                            " ", "shift",  ""};

static const char * btnm_map4[] = {"V", "B", "N", "\n",
                            "M", ",", ".", "\n",
                            "/", ";", "'", "\n",
                            " ", "SHITF",  ""};

// class ClientCallbacks : public NimBLEClientCallbacks {
//   void onConnect(NimBLEClient *pClient) {
//     Serial.println("Connected");
//     pClient->updateConnParams(120, 120, 0, 60);
//     // char str[30];
//     // snprintf(str, 30, LV_SYMBOL_BLUETOOTH " Connected");
//     // ui_send_msg(str, 1000);
//   };

//   void onDisconnect(NimBLEClient *pClient) {
//     Serial.print(pClient->getPeerAddress().toString().c_str());
//     Serial.println(" Disconnected");
//     // char str[30];
//     // snprintf(str, 30, LV_SYMBOL_BLUETOOTH " Disconnected");
//     // ui_send_msg(str, 1000);
//     // NimBLEDevice::getScan()->start(scanTime, scanEndedCB);
//   };

//   bool onConnParamsUpdateRequest(NimBLEClient *pClient, const ble_gap_upd_params *params) {
//     if (params->itvl_min < 24) { /** 1.25ms units */
//       return false;
//     } else if (params->itvl_max > 40) { /** 1.25ms units */
//       return false;
//     } else if (params->latency > 2) { /** Number of intervals allowed to skip */
//       return false;
//     } else if (params->supervision_timeout > 100) { /** 10ms units */
//       return false;
//     }

//     return true;
//   };

//   /********************* Security handled here **********************
//   ****** Note: these are the same return values as defaults ********/
//   uint32_t onPassKeyRequest() {
//     Serial.println("Client Passkey Request");
//     /** return the passkey to send to the server */
//     return 123456;
//   };

//   bool onConfirmPIN(uint32_t pass_key) {
//     Serial.print("The passkey YES/NO number: ");
//     Serial.println(pass_key);
//     /** Return false if passkeys don't match. */
//     return true;
//   };

//   /** Pairing process complete, we can check the results in ble_gap_conn_desc */
//   void onAuthenticationComplete(ble_gap_conn_desc *desc) {
//     if (!desc->sec_state.encrypted) {
//       Serial.println("Encrypt connection failed - disconnecting");
//       /** Find the client with the connection handle provided in desc */
//       NimBLEDevice::getClientByID(desc->conn_handle)->disconnect();
//       return;
//     }
//   };
// };

static struct {
  lv_obj_t *dd_ssid;
  lv_obj_t *kb;
  lv_obj_t *msg;
  lv_timer_t *timer;
  String ssid_list;

  lv_obj_t *dd_ble_name;
  String ble_name_list;

  // ClientCallbacks clientCB;
  // NimBLEScanResults results;
  // std::vector<NimBLEAdvertisedDevice *> advDevice_vector;
} wireless_param;

// NimBLEClient *pClient = nullptr;
// NimBLERemoteService *pSvc = nullptr;
// NimBLERemoteCharacteristic *p2A6EChr = nullptr;
// NimBLERemoteCharacteristic *p2A6FChr = nullptr;

LV_FONT_DECLARE(alibaba_font);

static lv_obj_t *create_obj_with_label(lv_obj_t *cont, textarea_config_t *u);
static lv_obj_t *create_btn_on_label(lv_obj_t *cont,const char *text);

static void get_wifi_ssid_event_cb(lv_event_t *e);
static void connect_wifi_event_cb(lv_event_t *e);
static void ta_event_cb(lv_event_t *e);

// static void get_ble_name_event_cb(lv_event_t *e);
// static void connect_ble_event_cb(lv_event_t *e);

void app_wireless_load(lv_obj_t *cont) {
  textarea_config_t t1 = {
      .obj_create = lv_dropdown_create,
      .align = LV_ALIGN_TOP_LEFT,
      .x_ofs = 0,
      .y_ofs = 55,
      .text = "SSID: ",
  };
  wireless_param.dd_ssid = create_obj_with_label(cont, &t1);
  lv_dropdown_set_options(wireless_param.dd_ssid, wireless_param.ssid_list.c_str());

  lv_obj_t *btn = create_btn_on_label(cont, LV_SYMBOL_REFRESH);
  lv_obj_align_to(btn, wireless_param.dd_ssid, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
  lv_obj_add_event_cb(btn, get_wifi_ssid_event_cb, LV_EVENT_CLICKED, wireless_param.dd_ssid);

  textarea_config_t t2 = {
      .obj_create = lv_textarea_create,
      .align = LV_ALIGN_TOP_LEFT,
      .x_ofs = 0,
      .y_ofs = 95,
      .text = "PASS: ",
  };
  lv_obj_t *t2_label = lv_label_create(cont);
  lv_obj_align(t2_label, t2.align, t2.x_ofs, t2.y_ofs);
  lv_obj_set_style_text_color(t2_label, lv_color_white(), 0);
  lv_label_set_text(t2_label, t2.text);
  lv_obj_t *t2_obj = t2.obj_create(cont);
  lv_obj_set_size(t2_obj, 140, 35);
  lv_obj_align_to(t2_obj, t2_label, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
  lv_obj_set_style_outline_color(t2_obj, lv_color_white(), LV_STATE_FOCUS_KEY);

  ta2 = t2_obj;
  lv_textarea_set_one_line(ta2, true);
  lv_textarea_set_password_mode(ta2, true);

  btn = create_btn_on_label(cont, LV_SYMBOL_OK);
  lv_obj_align_to(btn, ta2, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
  lv_obj_add_event_cb(btn, connect_wifi_event_cb, LV_EVENT_CLICKED, ta2);

  textarea_config_t t3 = {
      .obj_create = lv_dropdown_create,
      .align = LV_ALIGN_TOP_LEFT,
      .x_ofs = 0,
      .y_ofs = 135,
      .text = "ble name: ",
  };
  lv_obj_t *t3_label = lv_label_create(cont);
  lv_obj_align(t3_label, t3.align, t3.x_ofs, t3.y_ofs);
  lv_obj_set_style_text_color(t3_label, lv_color_white(), 0);
  lv_label_set_text(t3_label, t3.text);
  lv_obj_t *t3_obj = t3.obj_create(cont);
  lv_obj_set_size(t3_obj, 140, 35);
  lv_obj_align_to(t3_obj, t3_label, LV_ALIGN_OUT_RIGHT_MID, -30, 30);
  lv_obj_set_style_outline_color(t3_obj, lv_color_white(), LV_STATE_FOCUS_KEY);

  lv_obj_t *ta3 = t3_obj;
  lv_dropdown_set_options(ta3, "");
  wireless_param.dd_ble_name = ta3;
  // Get device with bluetooth name
  btn = create_btn_on_label(cont, LV_SYMBOL_REFRESH);
  lv_obj_align_to(btn, ta3, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
  // lv_obj_add_event_cb(btn, get_ble_name_event_cb, LV_EVENT_CLICKED, NULL);
  // connect bluetooth
  btn = create_btn_on_label(cont, LV_SYMBOL_OK);
  lv_obj_align_to(btn, ta3, LV_ALIGN_OUT_RIGHT_MID, 10, 40);
  // lv_obj_add_event_cb(btn, connect_ble_event_cb, LV_EVENT_CLICKED, ta3);

  wireless_param.kb = lv_keyboard_create(cont);
  lv_obj_add_flag(wireless_param.kb, LV_OBJ_FLAG_HIDDEN);

  lv_obj_add_event_cb(ta2, ta_event_cb, LV_EVENT_CLICKED, (void *)t2_label); 
}

static lv_obj_t *create_obj_with_label(lv_obj_t *cont, textarea_config_t *u) {
  lv_obj_t *label = lv_label_create(cont);
  lv_obj_align(label, u->align, u->x_ofs, u->y_ofs);
  lv_obj_set_style_text_color(label, lv_color_white(), 0);
  lv_label_set_text(label, u->text);

  lv_obj_t *obj = u->obj_create(cont);
  lv_obj_set_size(obj, 140, 35);
  lv_obj_align_to(obj, label, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
  lv_obj_set_style_outline_color(obj, lv_color_white(), LV_STATE_FOCUS_KEY);

  return obj;
}

static lv_obj_t *create_btn_on_label(lv_obj_t *cont,const char *text) {

  lv_obj_t *btn = lv_btn_create(cont);
  lv_obj_set_size(btn, 40, 35);
  lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_GREY), 0);
  lv_obj_set_style_outline_color(btn, lv_color_white(), LV_STATE_FOCUS_KEY);

  lv_obj_t *label = lv_label_create(btn);
  lv_obj_center(label);
  lv_label_set_text(label, text);

  return btn;
}

static void get_wifi_ssid_event_cb(lv_event_t *e) {
  lv_obj_t *dd = (lv_obj_t *)lv_event_get_user_data(e);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  int n = WiFi.scanNetworks();
  wireless_param.ssid_list = "";
  for (int i = 0; i < n; i++) {
    wireless_param.ssid_list += WiFi.SSID(i);
    wireless_param.ssid_list += "\n";
  }
  lv_dropdown_set_options(dd, wireless_param.ssid_list.c_str());
}

static void connect_wifi_event_cb(lv_event_t *e) {

  lv_obj_t *password_obj = (lv_obj_t *)lv_event_get_user_data(e);
  lv_obj_t *dd = wireless_param.dd_ssid;
  char ssid[20];
  lv_dropdown_get_selected_str(dd, ssid, 20);
  const char *password = lv_textarea_get_text(password_obj);
  WiFi.disconnect();
  WiFi.begin(ssid, password);

  wireless_param.timer = lv_timer_create(
      [](lv_timer_t *t) {
        lv_obj_t *ip_label = (lv_obj_t *)t->user_data;
        if (WiFi.status() == WL_CONNECTED) {
          if (ip_label != nullptr)
            lv_label_set_text_fmt(ip_label, "IP : %s", WiFi.localIP().toString().c_str());

          lv_timer_del(wireless_param.timer);
        }
      },
      1000, wireless_param.msg);
}

extern void my_print(const char *buf);

void Input_box(void)
{

}

static void textarea_event_handler(lv_event_t * e)
{
    //lv_obj_t * ta = lv_event_get_target(e);
    //LV_LOG_USER("Enter was pressed. The current text is: %s", lv_textarea_get_text(ta));
}


static void btnm_event_handler(lv_event_t * e)
{
    static int uppercase_t = 0;
    lv_obj_t * obj = lv_event_get_target(e);
    lv_obj_t * ta = (lv_obj_t *)lv_event_get_user_data(e);
    const char * txt = lv_btnmatrix_get_btn_text(obj, lv_btnmatrix_get_selected_btn(obj));

    if(strcmp(txt, LV_SYMBOL_BACKSPACE) == 0) lv_textarea_del_char(ta);
    else if(strcmp(txt, LV_SYMBOL_NEW_LINE) == 0)
    {
        lv_event_send(ta, LV_EVENT_READY, NULL);
        const char * txt = lv_textarea_get_text(ta);
        lv_textarea_set_text(ta2, txt);
        lv_obj_t* obj_parent = lv_obj_get_parent(obj);
        lv_obj_del_delayed(obj_parent, 1);
    } 
    else if(strcmp(txt, "shift") == 0) 
    {
        lv_btnmatrix_set_map(obj, btnm_map4);
        uppercase_t = 1;
    }
    else if(strcmp(txt, "SHITF") == 0) 
    {
        lv_btnmatrix_set_map(obj, btnm_map3);
        uppercase_t = 0;
    }
    else 
    {
        if(strlen(txt) == 1 && txt[0] >= 'A' && txt[0] <= 'Z')
        {
            char str[2] = {0};
            str[0] = txt[0];
            if(!uppercase_t)
            {
                str[0] = txt[0] + 'a'-'A';
            }
            lv_textarea_add_text(ta, str);
        }
        else 
          lv_textarea_add_text(ta, txt);
    }
}

lv_obj_t * textarea_btnm = NULL;
static void textarea_right_right_cd(lv_event_t *e)
{
    static int btnm_map_t = 0;
    if((int)e->user_data == 1) 
    {
        btnm_map_t>0?btnm_map_t--:btnm_map_t = 3;
    }
    else if((int)e->user_data == 2) 
    {
        btnm_map_t<3?btnm_map_t++:btnm_map_t = 0;
    }
    char str[30] = {0};
    sprintf(str, "btnm_map_t %d, %d\n", btnm_map_t, (int)e->user_data);
    my_print(str);

    switch (btnm_map_t)
    {
      case 0:
          lv_btnmatrix_set_map(textarea_btnm, btnm_map0);
      break;
      case 1:
          lv_btnmatrix_set_map(textarea_btnm, btnm_map1);
      break;
      case 2:
          lv_btnmatrix_set_map(textarea_btnm, btnm_map2);
      break;
      case 3:
          lv_btnmatrix_set_map(textarea_btnm, btnm_map3);
      break;
    }
}

void ta_event_textarea(void)
{
    lv_obj_t * obj = lv_obj_create(lv_layer_top());
    lv_obj_set_size(obj, 240, 240);
    lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), 0);
    lv_obj_set_style_border_width(obj, 0, 0);  
    lv_obj_set_style_pad_all(obj, 0, 0); 

    lv_obj_t * ta = lv_textarea_create(obj);
    lv_obj_set_size(ta, 200, 35);
    lv_textarea_set_one_line(ta, true);
    lv_obj_align(ta, LV_ALIGN_TOP_MID, 0, 30);
    lv_obj_add_event_cb(ta, textarea_event_handler, LV_EVENT_READY, ta);
    lv_obj_add_state(ta, LV_STATE_FOCUSED);

    textarea_btnm = lv_btnmatrix_create(obj);
    lv_obj_set_size(textarea_btnm, 200, 150);
    lv_obj_align(textarea_btnm, LV_ALIGN_BOTTOM_MID, 0, -5);
    lv_obj_set_style_bg_color(textarea_btnm, lv_color_hex(0x000000), 0);
    lv_obj_set_style_border_opa(textarea_btnm, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_add_event_cb(textarea_btnm, btnm_event_handler, LV_EVENT_VALUE_CHANGED, ta);
    lv_obj_clear_flag(textarea_btnm, LV_OBJ_FLAG_CLICK_FOCUSABLE); 
    lv_btnmatrix_set_map(textarea_btnm, btnm_map0);

    lv_obj_t *right_btn = lv_btn_create(obj);
    lv_obj_align_to(right_btn, textarea_btnm, LV_ALIGN_RIGHT_MID, 25, -10);
    lv_obj_add_event_cb(right_btn, textarea_right_right_cd, LV_EVENT_CLICKED, (void *)2);
    lv_obj_set_style_bg_opa(right_btn, LV_OPA_0, 0);
    lv_obj_set_style_outline_color(right_btn, lv_color_white(), LV_STATE_FOCUS_KEY);
    lv_obj_set_style_border_opa(right_btn, LV_OPA_100, LV_PART_MAIN);
    lv_obj_set_size(right_btn, 33, 33);
    lv_obj_t *right_text = lv_label_create(right_btn);
    lv_obj_center(right_text);
    lv_label_set_text(right_text, LV_SYMBOL_RIGHT);

    lv_obj_t *left_btn = lv_btn_create(obj);
    lv_obj_align_to(left_btn, textarea_btnm, LV_ALIGN_LEFT_MID, -35, -10);
    lv_obj_add_event_cb(left_btn, textarea_right_right_cd, LV_EVENT_CLICKED, (void *)1);
    lv_obj_set_style_bg_opa(left_btn, LV_OPA_0, 0);
    lv_obj_set_style_outline_color(left_btn, lv_color_white(), LV_STATE_FOCUS_KEY);
    lv_obj_set_size(left_btn, 33, 33);
    lv_obj_t *left_text = lv_label_create(left_btn);
    lv_obj_center(left_text);
    lv_label_set_text(left_text, LV_SYMBOL_LEFT);
}

static void ta_event_cb(lv_event_t *e) {
  my_print("\nta_event_cb\n");
  ta_event_textarea();
}

/** Callback to process the results of the last scan or restart it */
// void scanEndedCB(NimBLEScanResults results) {
//   Serial.println("Scan Ended");
//   wireless_param.results = results;
// }

// /** Define a class to handle the callbacks when advertisments are received */
// class AdvertisedDeviceCallbacks : public NimBLEAdvertisedDeviceCallbacks {

//   void onResult(NimBLEAdvertisedDevice *advertisedDevice) {
//     Serial.print("Advertised Device found: ");
//     Serial.println(advertisedDevice->toString().c_str());
//     if (advertisedDevice->haveName()) {
//       wireless_param.ble_name_list += advertisedDevice->getName().c_str();
//       wireless_param.ble_name_list += "\n";
//       wireless_param.advDevice_vector.push_back(advertisedDevice);
//       lv_dropdown_set_options(wireless_param.dd_ble_name, wireless_param.ble_name_list.c_str());
//     }
//   };
// };
// /** Notification / Indication receiving handler callback */
// void notifyCB(NimBLERemoteCharacteristic *pRemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify) {
//   std::string str = (isNotify == true) ? "Notification" : "Indication";
//   str += " from ";
//   /** NimBLEAddress and NimBLEUUID have std::string operators */
//   str += std::string(pRemoteCharacteristic->getRemoteService()->getClient()->getPeerAddress());
//   str += ": Service = " + std::string(pRemoteCharacteristic->getRemoteService()->getUUID());
//   str += ", Characteristic = " + std::string(pRemoteCharacteristic->getUUID());
//   str += ", Value = " + std::string((char *)pData, length);
//   Serial.println(str.c_str());
//   /* Need to judge the flag bit to decide whether to send the update event */
//   /* To prevent the reset after the exit is still refreshed. */
//   float fData = String(pData, length).toFloat();
//   if (pRemoteCharacteristic->getUUID() == NimBLEUUID("2A6E")) {
//     lv_msg_send(MSG_BLE_SEND_DATA_1, &fData);
//   } else if (pRemoteCharacteristic->getUUID() == NimBLEUUID("2A6F")) {
//     lv_msg_send(MSG_BLE_SEND_DATA_2, &fData);
//   }
// }

// static void get_ble_name_event_cb(lv_event_t *e) {

//   NimBLEDevice::init("");
//   NimBLEDevice::setSecurityAuth(BLE_SM_PAIR_AUTHREQ_SC);
//   NimBLEDevice::setPower(ESP_PWR_LVL_P9);
//   NimBLEScan *pScan = NimBLEDevice::getScan();
//   pScan->setAdvertisedDeviceCallbacks(new AdvertisedDeviceCallbacks());

//   pScan->setInterval(45);
//   pScan->setWindow(15);

//   pScan->setActiveScan(true);

//   pScan->start(10, scanEndedCB);
//   wireless_param.ble_name_list.clear();
//   wireless_param.advDevice_vector.clear();
// }

// static void connect_ble_event_cb(lv_event_t *e) {
//   char ble_name[20];
//   uint16_t dev_num;
//   lv_dropdown_get_selected_str(wireless_param.dd_ble_name, ble_name, 20);
//   dev_num = lv_dropdown_get_selected(wireless_param.dd_ble_name);
//   Serial.println("dev_num : " + String(dev_num));

//   NimBLEScan *pScan = NimBLEDevice::getScan();
//   pScan->stop();

//   /** Check if we have a client we should reuse first **/
//   if (NimBLEDevice::getClientListSize()) {
//     /** Special case when we already know this device, we send false as the
//      *  second argument in connect() to prevent refreshing the service database.
//      *  This saves considerable time and power.
//      */
//     pClient = NimBLEDevice::getClientByPeerAddress(wireless_param.advDevice_vector[dev_num]->getAddress());
//     if (pClient) {
//       if (!pClient->connect(wireless_param.advDevice_vector[dev_num], false)) {
//         Serial.println("Reconnect failed");
//         return;
//       }
//       Serial.println("Reconnected client");
//     }
//     /** We don't already have a client that knows this device,
//      *  we will check for a client that is disconnected that we can use.
//      */
//     else {
//       pClient = NimBLEDevice::getDisconnectedClient();
//     }
//   }
//   /** No client to reuse? Create a new one. */
//   if (!pClient) {
//     if (NimBLEDevice::getClientListSize() >= NIMBLE_MAX_CONNECTIONS) {
//       Serial.println("Max clients reached - no more connections available");
//       return;
//     }

//     pClient = NimBLEDevice::createClient();

//     Serial.println("New client created");

//     pClient->setClientCallbacks(&wireless_param.clientCB, false);
//     pClient->setConnectionParams(12, 12, 0, 51);
//     /** Set how long we are willing to wait for the connection to complete (seconds), default is 30. */
//     pClient->setConnectTimeout(5);

//     if (!pClient->connect(wireless_param.advDevice_vector[dev_num])) {
//       /** Created a client but failed to connect, don't need to keep it as it has no data */
//       NimBLEDevice::deleteClient(pClient);
//       Serial.println("Failed to connect, deleted client");
//       return;
//     }
//   }

//   pSvc = pClient->getService("1809");
//   char str[50];
//   snprintf(str, 50, LV_SYMBOL_BLUETOOTH " Connected Mac:");
//   strcmp(str, pClient->getConnInfo().getAddress().toString().c_str());
//   lv_label_set_text(wireless_param.msg, str);
//   pScan->clearResults();
//   if (pSvc != nullptr) { /* Determine whether to connect and identify the Bluetooth server */
//     p2A6EChr = pSvc->getCharacteristic("2A6E");
//     if (p2A6EChr) {
//       if (p2A6EChr->canNotify()) {
//         // if(!pChr->registerForNotify(notifyCB)) {
//         if (!p2A6EChr->subscribe(true, notifyCB)) {
//           /** Disconnect if subscribe failed */
//           pSvc->getClient()->disconnect();
//         }
//       }
//     }
//     p2A6FChr = pSvc->getCharacteristic("2A6F");
//     if (p2A6FChr) {
//       if (p2A6FChr->canNotify()) {
//         // if(!pChr->registerForNotify(notifyCB)) {
//         if (!p2A6FChr->subscribe(true, notifyCB)) {
//           /** Disconnect if subscribe failed */
//           pSvc->getClient()->disconnect();
//         }
//       }
//     }
//   }
// }

app_t app_wireless = {
    .setup_func_cb = app_wireless_load,
    .exit_func_cb = nullptr,
    .user_data = nullptr,
};