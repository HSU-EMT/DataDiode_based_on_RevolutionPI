#pins setting for the first pmod-rs485 
set_property PACKAGE_PIN V15 [get_ports {rx_enable_0_tri_o[0]}]
set_property PACKAGE_PIN T10 [get_ports {tx_enable_0_tri_o[0]}]
set_property PACKAGE_PIN T11 [get_ports uart_rtl_0_rxd]
set_property PACKAGE_PIN W15 [get_ports uart_rtl_0_txd]
set_property IOSTANDARD LVCMOS33 [get_ports {rx_enable_0_tri_o[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {tx_enable_0_tri_o[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports uart_rtl_0_rxd]
set_property IOSTANDARD LVCMOS33 [get_ports uart_rtl_0_txd]

#pins setting for the second pmod-rs485 
set_property IOSTANDARD LVCMOS33 [get_ports {rx_enable_1_tri_o[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {tx_enable_1_tri_o[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports uart_rtl_1_rxd]
set_property IOSTANDARD LVCMOS33 [get_ports uart_rtl_1_txd]
set_property PACKAGE_PIN T14 [get_ports {rx_enable_1_tri_o[0]}]
set_property PACKAGE_PIN R14 [get_ports {tx_enable_1_tri_o[0]}]
set_property PACKAGE_PIN P14 [get_ports uart_rtl_1_rxd]
set_property PACKAGE_PIN T15 [get_ports uart_rtl_1_txd]

