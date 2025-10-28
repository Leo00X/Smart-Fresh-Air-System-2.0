<template>
	<view class="container">
		<view class="header-status">
			<text class="update-time">数据上次更新: {{ lastUpdateTime }}</text>
		</view>

		<view class="panel-card global-control">
			<view class="panel-title">全局模式</view>
			<uni-segmented-control :current="globalModeIndex" :values="globalModeItems" @clickItem="onGlobalModeChange"
				style-type="button" active-color="#007aff"></uni-segmented-control>
		</view>

		<view class="panel-card">
			<view class="panel-title">中央控制主机</view>
			<uni-grid :column="4" :showBorder="false" :square="false">
				<uni-grid-item>
					<view class="grid-item">
						<image class="grid-icon" src="/static/temp.png"></image>
						<text class="grid-value">{{ formatData(deviceData.host_temperature, '°C') }}</text>
						<text class="grid-label">温度</text>
					</view>
				</uni-grid-item>
				<uni-grid-item>
					<view class="grid-item">
						<image class="grid-icon" src="/static/humi.png"></image>
						<text class="grid-value">{{ formatData(deviceData.host_humidity, '%') }}</text>
						<text class="grid-label">湿度</text>
					</view>
				</uni-grid-item>
				<uni-grid-item>
					<view class="grid-item">
						<image class="grid-icon" src="/static/pm25.png"></image>
						<text class="grid-value">{{ formatData(deviceData.host_pm25_value, '') }}</text>
						<text class="grid-label">PM2.5</text>
					</view>
				</uni-grid-item>
				<uni-grid-item>
					<view class="grid-item">
						<image class="grid-icon" src="/static/hcho.png"></image>
						<text class="grid-value">{{ formatData(deviceData.host_formaldehyde_ppb, 'ppb') }}</text>
						<text class="grid-label">甲醛</text>
					</view>
				</uni-grid-item>
				<uni-grid-item>
					<view class="grid-item">
						<image class="grid-icon" src="/static/smog.png"></image>
						<text class="grid-value">{{ formatData(deviceData.host_smog_ppm, 'ppm') }}</text>
						<text class="grid-label">烟雾</text>
					</view>
				</uni-grid-item>
				<uni-grid-item>
					<view class="grid-item">
						<image class="grid-icon" src="/static/gas.png"></image>
						<text class="grid-value">{{ formatData(deviceData.host_gas_ppm, 'ppm') }}</text>
						<text class="grid-label">燃气</text>
					</view>
				</uni-grid-item>
			</uni-grid>
			<view class="status-box" :class="hostStatus.safe ? 'safe' : 'warning'">
				<text>主机状态：{{ hostStatus.text }}</text>
			</view>
		</view>

		<view class="slave-section-title">分布式新风机</view>

		<view class="panel-card slave-card">
			<view class="panel-title">新风机①</view>
			<view class="slave-data-grid">
				<text>温度: {{ formatData(deviceData.slave1_temperature, '°C') }}</text>
				<text>湿度: {{ formatData(deviceData.slave1_humidity, '%') }}</text>
				<text>烟雾: {{ formatData(deviceData.slave1_smog_ppm, 'ppm') }}</text>
				<text>CO: {{ formatData(deviceData.slave1_co_ppm, 'ppm') }}</text>
				<text>AQI: {{ formatData(deviceData.slave1_aqi_ppm, 'ppm') }}</text>
				<text class="status-text safe">状态: 安全</text>
			</view>
			<view class="control-wrapper">
				<text class="control-label">风机速度: {{ deviceData.slave1_fan_speed }}%</text>
				<slider class="fan-slider" :value="deviceData.slave1_fan_speed" min="0" max="100" step="1"
					@change="onFanSpeedChange(1, $event)" block-size="20" activeColor="#007aff" />
			</view>
		</view>

		<view class="panel-card slave-card">
			<view class="panel-title">新风机②</view>
			<view class="slave-data-grid">
				<text>温度: {{ formatData(deviceData.slave2_temperature, '°C') }}</text>
				<text>湿度: {{ formatData(deviceData.slave2_humidity, '%') }}</text>
				<text>烟雾: {{ formatData(deviceData.slave2_smog_ppm, 'ppm') }}</text>
				<text>CO: {{ formatData(deviceData.slave2_co_ppm, 'ppm') }}</text>
				<text>AQI: {{ formatData(deviceData.slave2_aqi_ppm, 'ppm') }}</text>
				<text class="status-text safe">状态: 安全</text>
			</view>
			<view class="control-wrapper">
				<text class="control-label">风机速度: {{ deviceData.slave2_fan_speed }}%</text>
				<slider class="fan-slider" :value="deviceData.slave2_fan_speed" min="0" max="100" step="1"
					@change="onFanSpeedChange(2, $event)" block-size="20" activeColor="#007aff" />
			</view>
		</view>

		<view class="panel-card slave-card">
			<view class="panel-title">新风机③</view>
			<view class="slave-data-grid">
				<text>温度: {{ formatData(deviceData.slave3_temperature, '°C') }}</text>
				<text>湿度: {{ formatData(deviceData.slave3_humidity, '%') }}</text>
				<text>烟雾: {{ formatData(deviceData.slave3_smog_ppm, 'ppm') }}</text>
				<text>CO: {{ formatData(deviceData.slave3_co_ppm, 'ppm') }}</text>
				<text>AQI: {{ formatData(deviceData.slave3_aqi_ppm, 'ppm') }}</text>
				<text class="status-text safe">状态: 安全</text>
			</view>
			<view class="control-wrapper">
				<text class="control-label">风机速度: {{ deviceData.slave3_fan_speed }}%</text>
				<slider class="fan-slider" :value="deviceData.slave3_fan_speed" min="0" max="100" step="1"
					@change="onFanSpeedChange(3, $event)" block-size="20" activeColor="#007aff" />
			</view>
		</view>
	</view>
</template>

<script>
	// (此 <script> 部分与上一版能正常连接的完全相同，无需改动)
	import { createCommonToken } from '@/key.js'; 
	
	export default {
		data() {
			return {
				PROID: "sMFCi53wHH",
				DEVICE_NAME: "air_01",
				token: '',
				lastUpdateTime: "加载中...",
				globalModeIndex: 0, 
				globalModeItems: ['自动', '手动', '睡眠'],
				dataTimer: null, 
				deviceData: {
					"global_fan_mode": 0,
					"host_formaldehyde_ppb": '...',
					"host_gas_ppm": '...',
					"host_humidity": '...',
					"host_pm25_value": '...',
					"host_smog_ppm": '...',
					"host_temperature": '...',
					"slave1_aqi_ppm": '...',
					"slave1_co_ppm": '...',
					"slave1_fan_speed": 0,
					"slave1_humidity": '...',
					"slave1_smog_ppm": '...',
					"slave1_temperature": '...',
					"slave2_aqi_ppm": '...',
					"slave2_co_ppm": '...',
					"slave2_fan_speed": 0,
					"slave2_humidity": '...',
					"slave2_smog_ppm": '...',
					"slave2_temperature": '...',
					"slave3_aqi_ppm": '...',
					"slave3_co_ppm": '...',
					"slave3_fan_speed": 0,
					"slave3_humidity": '...',
					"slave3_smog_ppm": '...',
					"slave3_temperature": '...'
				}
			}
		},
		onLoad() {
			try {
				const params = {
					author_key: 'MtSNFVVvy2EtY4CWtbyku+HF0mugavWVkGTAcok3Nnp1/UM03xkkEZr+EBPFbPeR', 
					version: '2022-05-01', 
					user_id: '441225', 
				}
				this.token = createCommonToken(params); 
				console.log("Detail.vue: Token 已生成");
			} catch (error) {
				console.error("Token 生成失败:", error);
				uni.showToast({ title: 'Token生成失败', icon: 'error' });
			}
		},
		onShow() {
			if (!this.token) {
				console.error("Token 为空, 无法获取数据");
				return;
			}
			this.fetchDeviceData();
			this.dataTimer = setInterval(() => {
				this.fetchDeviceData();
			}, 10000); 
		},
		onHide() {
			if(this.dataTimer) {
				clearInterval(this.dataTimer);
				this.dataTimer = null;
			}
		},
		onUnload() {
			if(this.dataTimer) {
				clearInterval(this.dataTimer);
				this.dataTimer = null;
			}
		},
		computed: {
			hostStatus() {
				if (this.deviceData.host_pm25_value > 75 || this.deviceData.host_formaldehyde_ppb > 100) {
					return { safe: false, text: '污染' };
				}
				if (this.deviceData.host_gas_ppm > 10 || this.deviceData.host_smog_ppm > 10) {
					return { safe: false, text: '警告' };
				}
				return { safe: true, text: '安全' };
			}
		},
		methods: {
			formatData(value, unit) {
				if (value === null || value === undefined || value === '...') {
					return 'N/A';
				}
				return value + ' ' + unit;
			},
			fetchDeviceData() {
				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/query-device-property', 
					method: 'GET',
					data: { 
						product_id: this.PROID,
						device_name: this.DEVICE_NAME
					},
					header: { 
						'authorization': this.token 
					},
					success: (res) => { 
						if (res.data && res.data.data && Array.isArray(res.data.data)) {
							this.updateDeviceData(res.data.data); 
							this.lastUpdateTime = new Date().toLocaleTimeString();
						} else { 
							console.error("数据拉取失败:", res.data);
						}
					},
					fail: (err) => { 
						console.error("网络请求失败:", err.errMsg);
					}
				});
			},
			updateDeviceData(apiDataArray) {
				const findPropertyValue = (identifier) => {
					const prop = apiDataArray.find(p => p.identifier === identifier || p.id === identifier); 
					return prop ? prop.value : null; 
				};
				for (const key in this.deviceData) {
					const value = findPropertyValue(key);
					if (value !== null) {
						this.deviceData[key] = value;
					}
				}
				this.globalModeIndex = this.deviceData.global_fan_mode;
			},
			onGlobalModeChange(e) {
				if (this.globalModeIndex === e.currentIndex) return;
				this.globalModeIndex = e.currentIndex;
				let modeValue = e.currentIndex; 
				console.log(`设置全局模式为: ${this.globalModeItems[modeValue]} (${modeValue})`);
				this.postDeviceProperty({ "global_fan_mode": modeValue });
			},
			onFanSpeedChange(slaveIndex, e) {
				let speed = e.detail.value;
				let propertyIdentifier = `slave${slaveIndex}_fan_speed`; 
				this.deviceData[propertyIdentifier] = speed;
				console.log(`设置新风机 ${slaveIndex} 速度为: ${speed}%`);
				let postData = {};
				postData[propertyIdentifier] = speed;
				this.postDeviceProperty(postData);
			},
			postDeviceProperty(properties) {
				console.log("准备下发指令:", properties);
				uni.showLoading({ title: '指令下发中...' });
				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/set-device-property', 
					method: 'POST',
					data: {
						product_id: this.PROID,
						device_name: this.DEVICE_NAME,
						params: properties 
					},
					header: { 
						'authorization': this.token 
					},
					success: (res) => {
						console.log(`设置 ${Object.keys(properties)[0]} 成功. API响应:`, res.data);
						if (res.data.code === 0) {
							uni.showToast({ title: '设置成功', icon: 'success' });
							setTimeout(() => {
								this.fetchDeviceData();
							}, 1000);
						} else {
							uni.showToast({ title: res.data.msg || '设置失败', icon: 'none' });
						}
					},
					fail: (err) => {
						console.error(`设置失败. 错误:`, err.errMsg);
						uni.showToast({ title: '设置失败', icon: 'none' });
					},
					complete: () => {
						uni.hideLoading();
					}
				});
			}
		}
	}
</script>

<style lang="scss">
	/* (此 <style> 部分与上一版能正常连接的完全相同，无需改动) */
	.container {
		display: flex;
		flex-direction: column;
		padding: 15px;
		background-color: #f8f8f8;
		min-height: 100vh;
		padding-bottom: 30px; 
	}
	.header-status {
		display: flex;
		justify-content: flex-end;
		align-items: center;
		padding: 0 5px 15px 5px;
		.update-time {
			font-size: 12px;
			color: #999;
		}
	}
	.panel-card {
		background-color: #ffffff;
		border-radius: 8px;
		padding: 15px;
		margin-bottom: 15px;
		box-shadow: 0 2px 8px rgba(0, 0, 0, 0.05);
	}
	.panel-title {
		font-size: 16px;
		font-weight: bold;
		margin-bottom: 15px;
		color: #333;
	}
	.global-control {
		padding-bottom: 20px;
	}
	.grid-item {
		display: flex;
		flex-direction: column;
		align-items: center;
		justify-content: center;
		height: 100%;
		padding: 10px 0;
	}
	.grid-icon {
		width: 28px;
		height: 28px;
		margin-bottom: 8px;
	}
	.grid-value {
		font-size: 16px;
		font-weight: bold;
		color: #007aff;
	}
	.grid-label {
		font-size: 12px;
		color: #666;
		margin-top: 4px;
	}
	.status-box {
		margin-top: 15px;
		padding: 10px;
		border-radius: 6px;
		text-align: center;
		font-weight: bold;
		font-size: 14px;
		&.safe {
			background-color: #e8f5e9; 
			color: #4caf50;
		}
		&.warning {
			background-color: #fff3e0; 
			color: #f57c00;
		}
	}
	.slave-section-title {
		font-size: 16px;
		font-weight: bold;
		color: #333;
		padding: 0 5px 10px 5px;
	}
	.slave-card {
		.slave-data-grid {
			display: grid;
			grid-template-columns: 1fr 1fr;
			gap: 12px;
			font-size: 14px;
			color: #555;
			.status-text {
				font-weight: bold;
				&.safe { color: #4caf50; }
				&.warning { color: #f57c00; }
			}
		}
	}
	.control-wrapper {
		margin-top: 20px;
		.control-label {
			font-size: 14px;
			color: #333;
		}
		.fan-slider {
			margin-top: 10px;
		}
	}
</style>