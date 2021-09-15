include $(TOPDIR)/rules.mk

PKG_NAME:=openvpn_ubus_info
PKG_RELEASE:=1
PKG_VERSION:=1.0.0

include $(INCLUDE_DIR)/package.mk

define Package/openvpn_ubus_info
	DEPENDS:=+libubus +libubox +libblobmsg-json
	CATEGORY:=Base system
	TITLE:=openvpn_ubus_info
endef

define Package/openvpn_ubus_info/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/openvpn_ubus_info $(1)/usr/bin
endef

$(eval $(call BuildPackage,openvpn_ubus_info))
