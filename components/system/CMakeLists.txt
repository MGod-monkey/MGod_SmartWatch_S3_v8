idf_component_register(SRC_DIRS
                    "."
                    # "wm8978"
                    "system_time"
                    "system_config"
                    INCLUDE_DIRS
                    "."
                    # "wm8978"
                    "system_time"
                    "system_config"
                    REQUIRES  esp_wifi nvs_flash lwip main
                    )
