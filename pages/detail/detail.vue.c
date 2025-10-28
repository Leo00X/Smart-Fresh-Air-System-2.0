<template>
	<view class="wrap">
		<view class="section-title">主机中控参数</view>
		<view class="last-updated-time" v-if="masterLastUpdateTime">
			数据上次更新: {{masterLastUpdateTime}}
		</view>
		<view class="dev-area">
			<view class="dev-cart">
				<view>
					<view class="dev-name">温度</view>
					<image class="dev-logo" src="../../static/temp.png" mode=""></image>
				</view>
				<view class="dev-data">{{master.temp}} ℃</view>
			</view>

			<view class="dev-cart">
				<view>
					<view class="dev-name">湿度</view>
					<image class="dev-logo" src="../../static/humi.png" mode=""></image>
				</view>
				<view class="dev-data">{{master.humi}} %</view>
			</view>

			<view class="dev-cart">
				<view>
					<view class="dev-name">烟雾 (主机)</view>
					<image class="dev-logo" src="../../static/smog.png" mode=""></image>
				</view>
				<view class="dev-data">{{master.smog}}<span class="unit-ppm"> PPM</span></view>
			</view>

			<view class="dev-cart">
				<view>
					<view class="dev-name">可燃气体</view>
					<image class="dev-logo" src="../../static/MQ5.png" mode=""></image>
				</view>
				<view class="dev-data">{{master.MQ5}}<span class="unit-ppm"> PPM</span></view>
			</view>

			<view v-if="master.beep==true" class="dev-cart alarm-cart danger-bg">
				危险
			</view>
			<view v-else class="dev-cart alarm-cart safe-bg">
				安全
			</view>

			<view class="dev-cart breach-count-cart">
				<view>
					<view class="dev-name">主机超限次数</view>
					<view class="dev-data count-data">{{master.thresholdBreachCount}}</view>
				</view>
			</view>
		</view>

		<view class="section-title">从机监测点</view>

		<view class="slave-summary-card">
			<view class="summary-title">三从机数据汇总</view>
			<view class="summary-grid">
				<view class="summary-item">
					<text class="summary-label">总温度:</text>
					<text class="summary-value">{{slavesTotalTemp}} ℃</text>
				</view>
				<view class="summary-item">
					<text class="summary-label">平均温度:</text>
					<text class="summary-value">{{slavesAverageTemp}} ℃</text>
				</view>
				<view class="summary-item">
					<text class="summary-label">总湿度:</text>
					<text class="summary-value">{{slavesTotalHumi}} %</text>
				</view>
				<view class="summary-item">
					<text class="summary-label">平均湿度:</text>
					<text class="summary-value">{{slavesAverageHumi}} %</text>
				</view>
				<view class="summary-item">
					<text class="summary-label">总烟雾(PPM):</text>
					<text class="summary-value">{{slavesTotalSmog}}</text>
				</view>
				<view class="summary-item">
					<text class="summary-label">平均烟雾(PPM):</text>
					<text class="summary-value">{{slavesAverageSmog}}</text>
				</view>
				<view class="summary-item">
					<text class="summary-label">总CO(PPM):</text>
					<text class="summary-value">{{slavesTotalCO}}</text>
				</view>
				<view class="summary-item">
					<text class="summary-label">平均CO(PPM):</text>
					<text class="summary-value">{{slavesAverageCO}}</text>
				</view>
			</view>
		</view>

		<view class="slave-devices-area">
			<view v-for="(slave, slaveIndex) in slaves" :key="slave.id" class="slave-device-card">
				<view class="slave-device-header">
					<text class="slave-device-name">{{ slave.name }}</text>
				</view>
				<view class="slave-parameters">
					<view class="slave-param-item">
						<text class="param-label">温度:</text>
						<text class="param-value">{{slave.temp}} ℃</text>
					</view>
					<view class="slave-param-item">
						<text class="param-label">湿度:</text>
						<text class="param-value">{{slave.humi}} %</text>
					</view>
					<view class="slave-param-item">
						<text class="param-label">烟雾:</text>
						<text class="param-value">{{slave.smog}} PPM</text>
					</view>
					<view class="slave-param-item">
						<text class="param-label">CO:</text>
						<text class="param-value">{{slave.co}} PPM</text>
					</view>
					<view class="slave-param-item">
						<text class="param-label">空气质量:</text>
						<text class="param-value">{{slave.airQuality}}</text>
					</view>
					<view class="slave-param-item alarm-status-item"
						:class="{'danger-bg': slave.isBeeping, 'slave-safe-bg': !slave.isBeeping}">
						<text class="param-label">状态:</text>
						<text class="param-value alarm-text">{{slave.isBeeping ? '危险' : '安全'}}</text>
					</view>
					</view>

				<view class="slave-actions-bar">
					<view class="breach-count-inline-display">
						<text class="action-bar-label">超限次数:</text>
						<text class="action-bar-value count-data">{{slave.thresholdBreachCount}}</text>
					</view>
					<button class="toggle-button" size="mini" @click="toggleSlaveThresholdSettings(slaveIndex)">
						{{ slave.showThresholdSettings ? '隐藏阈值设置' : '设置预警阈值' }}
					</button>
				</view>
				
				<view v-if="slave.showThresholdSettings" class="threshold-settings">
					<view class="threshold-item">
						<text class="threshold-label">温度上限 (°C):</text>
						<slider class="threshold-slider" :value="slave.thresholds.temp_upper" @change="handleSlaveThresholdChange(slaveIndex, 'temp_upper', $event)" min="0" max="60" step="1" show-value block-size="20"/>
					</view>
					<view class="threshold-item">
						<text class="threshold-label">温度下限 (°C):</text>
						<slider class="threshold-slider" :value="slave.thresholds.temp_lower" @change="handleSlaveThresholdChange(slaveIndex, 'temp_lower', $event)" min="-20" max="40" step="1" show-value block-size="20"/>
					</view>
					<view class="threshold-item">
						<text class="threshold-label">湿度上限 (%):</text>
						<slider class="threshold-slider" :value="slave.thresholds.humi_upper" @change="handleSlaveThresholdChange(slaveIndex, 'humi_upper', $event)" min="0" max="100" step="1" show-value block-size="20"/>
					</view>
					<view class="threshold-item">
						<text class="threshold-label">湿度下限 (%):</text>
						<slider class="threshold-slider" :value="slave.thresholds.humi_lower" @change="handleSlaveThresholdChange(slaveIndex, 'humi_lower', $event)" min="0" max="100" step="1" show-value block-size="20"/>
					</view>
					<view class="threshold-item">
						<text class="threshold-label">烟雾上限 (PPM):</text>
						<slider class="threshold-slider" :value="slave.thresholds.smog_upper" @change="handleSlaveThresholdChange(slaveIndex, 'smog_upper', $event)" min="0" max="200" step="0.1" show-value block-size="20"/>
					</view>
					<view class="threshold-item">
						<text class="threshold-label">CO上限 (PPM):</text>
						<slider class="threshold-slider" :value="slave.thresholds.co_upper" @change="handleSlaveThresholdChange(slaveIndex, 'co_upper', $event)" min="0" max="200" step="0.1" show-value block-size="20"/>
					</view>
				</view>
			</view>
		</view>

		<view class="section-title">全局控制</view>
		<view class="controls-area">
			<view class="device-cart-l">
				<view>
					<view class="dev-name">数据更新频率 (秒)</view>
				</view>
				<view class="ctrl-slider">
					<slider :value="refreshInterval / 1000" @change="sliderChangeRefreshInterval" min="1" max="60" step="1"
						block-size="20" show-value />
				</view>
			</view>
		</view>
	</view>
</template>

<script>
	// 引入创建通用令牌函数, 请确保路径 '@/' 指向您的项目根目录下的 'key.js' 文件
	// **修改**: 将 CommonJS 的 require 修改为 ES6 的 import
	import { createCommonToken } from '@/key.js'; 

	// --- 全局配置常量 ---
	// **重要**: 请将以下 MASTER_PRODUCT_ID 和 MASTER_DEVICE_NAME 替换为您的OneNET平台上的实际主机产品ID和设备名称
	const MASTER_PRODUCT_ID = '6Tn8NKV9sN'; 
	const MASTER_DEVICE_NAME = 'ht32';  

	// 从机设备配置数组
	// Note: Initial threshold values for smog_upper are already floats. CO_upper were integers.
	// The slider step change will allow UI to reflect and set these with decimals.
	const SLAVE_CONFIG = [
		{
			id: 'slave001',
			displayName: '监测点 A', 
			apiIdentifiers: { 
				temp: 'slave1_temp',      
				humi: 'slave1_humi',      
				smog: 'slave1_smog',      
				co: 'slave1_co',          
				airQuality: 'slave1_aqi',  
				beep: 'slave1_beep',      
				breach_count: 'slave1_breach_count', 
				temp_upper: 'slave1_temp_upper_threshold', 
				temp_lower: 'slave1_temp_lower_threshold', 
				humi_upper: 'slave1_humi_upper_threshold', 
				humi_lower: 'slave1_humi_lower_threshold', 
				smog_upper: 'slave1_smog_upper_threshold', 
				co_upper:   'slave1_co_upper_threshold'   
			},
			thresholds: { 
				temp_upper: 35, temp_lower: 5, 
				humi_upper: 80, humi_lower: 20, 
				smog_upper: 2.0, // Already a float
				co_upper: 15     // Will now be settable as float via UI
			}
		},
		{
			id: 'slave002',
			displayName: '监测点 B', 
			apiIdentifiers: {
				temp: 'slave2_temp',      
				humi: 'slave2_humi',      
				smog: 'slave2_smog',      
				co: 'slave2_co',          
				airQuality: 'slave2_aqi',  
				beep: 'slave2_beep',      
				breach_count: 'slave2_breach_count',
				temp_upper: 'slave2_temp_upper_threshold', 
				temp_lower: 'slave2_temp_lower_threshold', 
				humi_upper: 'slave2_humi_upper_threshold', 
				humi_lower: 'slave2_humi_lower_threshold', 
				smog_upper: 'slave2_smog_upper_threshold', 
				co_upper:   'slave2_co_upper_threshold'   
			},
			thresholds: {
				temp_upper: 32, temp_lower: 8,
				humi_upper: 75, humi_lower: 25,
				smog_upper: 1.8, // Already a float
				co_upper: 12
			}
		},
		{
			id: 'slave003',
			displayName: '监测点 C', 
			apiIdentifiers: {
				temp: 'slave3_temp',      
				humi: 'slave3_humi',      
				smog: 'slave3_smog',      
				co: 'slave3_co',          
				airQuality: 'slave3_aqi',  
				beep: 'slave3_beep',      
				breach_count: 'slave3_breach_count',
				temp_upper: 'slave3_temp_upper_threshold', 
				temp_lower: 'slave3_temp_lower_threshold', 
				humi_upper: 'slave3_humi_upper_threshold', 
				humi_lower: 'slave3_humi_lower_threshold', 
				smog_upper: 'slave3_smog_upper_threshold', 
				co_upper:   'slave3_co_upper_threshold'   
			},
			thresholds: {
				temp_upper: 30, temp_lower: 10,
				humi_upper: 70, humi_lower: 30,
				smog_upper: 1.5, // Already a float
				co_upper: 10
			}
		}
	];

	export default {
		data() {
			return {
				master: {
					temp: '', 
					humi: '', 
					smog: '', 
					MQ5: '',  
					beep: false, 
					thresholdBreachCount: 0 
				},
				masterLastUpdateTime: '', 
				token: '',
				refreshInterval: 2000, 
				intervalId: null,      
				slaves: SLAVE_CONFIG.map(config => ({
					id: config.id,
					name: config.displayName,
					temp: '',       
					humi: '',       
					smog: '',       
					co: '',         
					airQuality: '', 
					isBeeping: false, 
					thresholds: JSON.parse(JSON.stringify(config.thresholds)), 
					showThresholdSettings: false, 
					thresholdBreachCount: 0 
				}))
			}
		},
		computed: { 
			slavesTotalTemp() { 
				return this.slaves.reduce((sum, slave) => {
					const val = parseFloat(slave.temp);
					return sum + (isNaN(val) ? 0 : val);
				}, 0).toFixed(1); 
			},
			slavesAverageTemp() { 
				const validSlaves = this.slaves.filter(slave => !isNaN(parseFloat(slave.temp))); 
				if (validSlaves.length === 0) return 'N/A'; 
				const sum = validSlaves.reduce((s, slave) => s + parseFloat(slave.temp), 0);
				return (sum / validSlaves.length).toFixed(1); 
			},
			slavesTotalHumi() { 
				return this.slaves.reduce((sum, slave) => {
					const val = parseFloat(slave.humi);
					return sum + (isNaN(val) ? 0 : val);
				}, 0).toFixed(1);
			},
			slavesAverageHumi() { 
				const validSlaves = this.slaves.filter(slave => !isNaN(parseFloat(slave.humi)));
				if (validSlaves.length === 0) return 'N/A';
				const sum = validSlaves.reduce((s, slave) => s + parseFloat(slave.humi), 0);
				return (sum / validSlaves.length).toFixed(1);
			},
			slavesTotalSmog() { 
				return this.slaves.reduce((sum, slave) => {
					const val = parseFloat(slave.smog);
					return sum + (isNaN(val) ? 0 : val);
				}, 0).toFixed(2); 
			},
			slavesAverageSmog() { 
				const validSlaves = this.slaves.filter(slave => !isNaN(parseFloat(slave.smog)));
				if (validSlaves.length === 0) return 'N/A';
				const sum = validSlaves.reduce((s, slave) => s + parseFloat(slave.smog), 0);
				return (sum / validSlaves.length).toFixed(2);
			},
			slavesTotalCO() { 
				return this.slaves.reduce((sum, slave) => {
					const val = parseFloat(slave.co);
					return sum + (isNaN(val) ? 0 : val);
				}, 0).toFixed(2);
			},
			slavesAverageCO() { 
				const validSlaves = this.slaves.filter(slave => !isNaN(parseFloat(slave.co)));
				if (validSlaves.length === 0) return 'N/A';
				const sum = validSlaves.reduce((s, slave) => s + parseFloat(slave.co), 0);
				return (sum / validSlaves.length).toFixed(2);
			}
		},
		onLoad() { 
			// **重要**: 请将以下 author_key 和 user_id 替换为您的实际OneNET平台的用户级密钥和用户ID
			const params = {
				author_key: 'MtSNFVVvy2EtY4CWtbyku+HF0mugavWVkGTAcok3Nnp1/UM03xkkEZr+EBPFbPeR', 
				version: '2022-05-01', 
				user_id: '441225', 
			}
			this.token = createCommonToken(params); 
		},
		onShow() { 
			this.fetchConsolidatedData(); 
			this.setupDataFetchInterval(); 
		},
		onHide() { 
			if (this.intervalId) {
				clearInterval(this.intervalId);
				this.intervalId = null;
			}
		},
		onUnload() { 
			if (this.intervalId) {
				clearInterval(this.intervalId);
				this.intervalId = null;
			}
		},
		methods: {
			setupDataFetchInterval() {
				if (this.intervalId) { 
					clearInterval(this.intervalId);
				}
				if (this.refreshInterval > 0) { 
					this.intervalId = setInterval(() => {
						this.fetchConsolidatedData(); 
					}, this.refreshInterval);
				}
			},
			sliderChangeRefreshInterval(e) {
				this.refreshInterval = e.detail.value * 1000; 
				this.setupDataFetchInterval(); 
			},
			handleSlaveThresholdChange(slaveIndex, thresholdKey, event) {
				// Values from slider are numbers if step is numeric, but parseFloat is robust
				const newValue = parseFloat(event.detail.value); 
				const slave = this.slaves[slaveIndex];

				if (slave && slave.thresholds) {
					// Use Vue.set or this.$set to ensure reactivity if adding new properties,
					// but for existing properties direct assignment is fine in Vue 2 methods
					// if the object itself is reactive. Here, thresholds object is part of reactive slave.
					slave.thresholds[thresholdKey] = newValue; 
					// Forcing a Vue update if direct assignment isn't picked up, though usually it is.
					// this.$forceUpdate(); // Usually not needed if 'thresholds' is properly reactive.

					const apiIdentifier = SLAVE_CONFIG[slaveIndex].apiIdentifiers[thresholdKey]; 
					
					if (!apiIdentifier) {
						console.error(`API identifier for threshold ${thresholdKey} not found for slave ${slave.name}`);
						uni.showToast({ title: `配置错误: ${thresholdKey}`, icon: 'none' });
						return;
					}

					let params_to_send = {
						[apiIdentifier]: newValue 
					};

					console.log(`准备发送到云端: ${slave.name} - ${apiIdentifier} = ${newValue} (Type: ${typeof newValue})`);

					uni.request({
						url: 'https://iot-api.heclouds.com/thingmodel/set-device-property', 
						method: 'POST',
						data: {
							product_id: MASTER_PRODUCT_ID,
							device_name: MASTER_DEVICE_NAME,
							params: params_to_send
						},
						header: { 'authorization': this.token },
						success: (res) => {
							console.log(`${slave.name} - ${thresholdKey} (${apiIdentifier}) 设置为 ${newValue} 成功. API响应:`, res.data);
							if (res.data.code !== 0 && res.data.code !== 200) { 
								 uni.showToast({ title: `${slave.name} ${thresholdKey} 设置失败: ${res.data.msg || '未知错误'}`, icon: 'none', duration: 3000 });
							} else {
								uni.showToast({ title: `${slave.name} ${thresholdKey.replace('_', ' ')} 已更新为 ${newValue}`, icon: 'success', duration: 1500 });
							}
						},
						fail: (err) => {
							console.error(`设置 ${slave.name} - ${thresholdKey} (${apiIdentifier}) 失败. 错误:`, err.errMsg);
							uni.showToast({ title: `${slave.name} ${thresholdKey} 设置失败`, icon: 'none' });
						}
					});
				}
			},
			toggleSlaveThresholdSettings(slaveIndex) {
				if (this.slaves[slaveIndex]) {
					this.slaves[slaveIndex].showThresholdSettings = !this.slaves[slaveIndex].showThresholdSettings;
				}
			},
			fetchConsolidatedData() {
				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/query-device-property', 
					method: 'GET',
					data: { 
						product_id: MASTER_PRODUCT_ID,
						device_name: MASTER_DEVICE_NAME
					},
					header: { 
						'authorization': this.token 
					},
					success: (res) => { 
						console.log("Unified API Response:", res.data); 
						if (res.data && res.data.data && Array.isArray(res.data.data)) {
							const properties = res.data.data; 

							const findPropertyValue = (identifier) => {
								const prop = properties.find(p => p.identifier === identifier || p.id === identifier);
								// Ensure to return the raw value for parsing, not 'N/A' yet
								return prop ? prop.value : null; 
							};

							this.master.temp = findPropertyValue('temp') !== null ? findPropertyValue('temp') : 'N/A'; 
							this.master.humi = findPropertyValue('humi') !== null ? findPropertyValue('humi') : 'N/A'; 
							this.master.smog = findPropertyValue('smog') !== null ? findPropertyValue('smog') : 'N/A'; 
							this.master.MQ5 = findPropertyValue('MQ5') !== null ? findPropertyValue('MQ5') : 'N/A';   
							// **重要**: 替换占位符 'master_breach_count_id_from_api' 为您在OneNET平台上为【主机超限次数】设置的实际标识符
							this.master.thresholdBreachCount = parseInt(findPropertyValue('master_breach_count_id_from_api')) || 0; 
							
							const masterBeepRaw = findPropertyValue('beep');
							const newMasterBeepState = masterBeepRaw === 'true' || masterBeepRaw === true; // Handle string 'true' or boolean true
							if (newMasterBeepState && !this.master.beep) { 
								uni.showToast({
									title: '主机状态：危险！', 
									icon: 'error', 
									duration: 3000 
								});
								uni.vibrateLong(); 
							}
							this.master.beep = newMasterBeepState; 

							this.slaves.forEach((slave, index) => {
								const config = SLAVE_CONFIG[index]; 
								slave.temp = findPropertyValue(config.apiIdentifiers.temp) !== null ? findPropertyValue(config.apiIdentifiers.temp) : 'N/A';
								slave.humi = findPropertyValue(config.apiIdentifiers.humi) !== null ? findPropertyValue(config.apiIdentifiers.humi) : 'N/A';
								slave.smog = findPropertyValue(config.apiIdentifiers.smog) !== null ? findPropertyValue(config.apiIdentifiers.smog) : 'N/A';
								slave.co = findPropertyValue(config.apiIdentifiers.co) !== null ? findPropertyValue(config.apiIdentifiers.co) : 'N/A';
								slave.airQuality = findPropertyValue(config.apiIdentifiers.airQuality) !== null ? findPropertyValue(config.apiIdentifiers.airQuality) : 'N/A';
								slave.thresholdBreachCount = parseInt(findPropertyValue(config.apiIdentifiers.breach_count)) || 0;
								
								// 从云端获取并更新每个从机的阈值, ensuring parseFloat is used
								const cloudTempUpper = findPropertyValue(config.apiIdentifiers.temp_upper);
								slave.thresholds.temp_upper = cloudTempUpper !== null ? parseFloat(cloudTempUpper) : config.thresholds.temp_upper;

								const cloudTempLower = findPropertyValue(config.apiIdentifiers.temp_lower);
								slave.thresholds.temp_lower = cloudTempLower !== null ? parseFloat(cloudTempLower) : config.thresholds.temp_lower;

								const cloudHumiUpper = findPropertyValue(config.apiIdentifiers.humi_upper);
								slave.thresholds.humi_upper = cloudHumiUpper !== null ? parseFloat(cloudHumiUpper) : config.thresholds.humi_upper;

								const cloudHumiLower = findPropertyValue(config.apiIdentifiers.humi_lower);
								slave.thresholds.humi_lower = cloudHumiLower !== null ? parseFloat(cloudHumiLower) : config.thresholds.humi_lower;
								
								const cloudSmogUpper = findPropertyValue(config.apiIdentifiers.smog_upper);
								slave.thresholds.smog_upper = cloudSmogUpper !== null ? parseFloat(cloudSmogUpper) : config.thresholds.smog_upper;
								
								const cloudCoUpper = findPropertyValue(config.apiIdentifiers.co_upper);
								slave.thresholds.co_upper   = cloudCoUpper !== null ? parseFloat(cloudCoUpper) : config.thresholds.co_upper;


								let breachedMessages = []; 
								const thresholds = slave.thresholds; 

								const tempValue = parseFloat(slave.temp);
								if (!isNaN(tempValue)) {
									if (thresholds.temp_upper !== undefined && tempValue > thresholds.temp_upper) {
										breachedMessages.push(`温度过高 (${tempValue}°C > ${thresholds.temp_upper}°C)`);
									}
									if (thresholds.temp_lower !== undefined && tempValue < thresholds.temp_lower) {
										breachedMessages.push(`温度过低 (${tempValue}°C < ${thresholds.temp_lower}°C)`);
									}
								}
								const humiValue = parseFloat(slave.humi);
								if (!isNaN(humiValue)) {
									if (thresholds.humi_upper !== undefined && humiValue > thresholds.humi_upper) {
										breachedMessages.push(`湿度过高 (${humiValue}% > ${thresholds.humi_upper}%)`);
									}
									if (thresholds.humi_lower !== undefined && humiValue < thresholds.humi_lower) {
										breachedMessages.push(`湿度过低 (${humiValue}% < ${thresholds.humi_lower}%)`);
									}
								}
								const smogValue = parseFloat(slave.smog);
								if (!isNaN(smogValue)) {
									// Ensure smog_upper threshold is a number for comparison
									const smogUpperThreshold = parseFloat(thresholds.smog_upper);
									if (!isNaN(smogUpperThreshold) && smogValue > smogUpperThreshold) {
										breachedMessages.push(`烟雾超标 (${smogValue}PPM > ${smogUpperThreshold.toFixed(1)}PPM)`);
									}
								}
								const coValue = parseFloat(slave.co);
								if (!isNaN(coValue)) {
									// Ensure co_upper threshold is a number for comparison
									const coUpperThreshold = parseFloat(thresholds.co_upper);
									if (!isNaN(coUpperThreshold) && coValue > coUpperThreshold) {
										breachedMessages.push(`CO超标 (${coValue}PPM > ${coUpperThreshold.toFixed(1)}PPM)`);
									}
								}
								
								const platformBeepRaw = findPropertyValue(config.apiIdentifiers.beep);
								const platformBeepForSlave = platformBeepRaw === 'true' || platformBeepRaw === true;
								const isThresholdBreached = breachedMessages.length > 0;
								const currentOverallAlarmState = platformBeepForSlave || isThresholdBreached;

								if (currentOverallAlarmState && !slave.isBeeping) { 
									let toastTitle = `${slave.name} `;
									if (isThresholdBreached) { 
										toastTitle += breachedMessages[0]; 
										if (breachedMessages.length > 1) {
											toastTitle += ` (等多项)`; 
										}
									} else if (platformBeepForSlave) { 
										toastTitle += "平台警报！";
									}
									
									uni.showToast({
										title: toastTitle,
										icon: 'error',
										duration: 4000 
									});
									uni.vibrateLong();
								}
								slave.isBeeping = currentOverallAlarmState; 
							});

							const now = new Date();
							this.masterLastUpdateTime = `${now.getFullYear()}-${String(now.getMonth() + 1).padStart(2, '0')}-${String(now.getDate()).padStart(2, '0')} ${String(now.getHours()).padStart(2, '0')}:${String(now.getMinutes()).padStart(2, '0')}:${String(now.getSeconds()).padStart(2, '0')}`;
							console.log('All data updated:', this.masterLastUpdateTime);

						} else { 
							console.error("API data format error or incomplete data:", res.data);
							uni.showToast({ title: '获取数据格式错误', icon: 'none' });
						}
					},
					fail: (err) => { 
						console.error("Failed to fetch unified data:", err.errMsg);
						uni.showToast({ title: `获取数据失败`, icon: 'none' });
					}
				});
			},
		}
	}
</script>

<style>
	.wrap {
		padding: 20rpx;
		background-color: #f4f4f4;
	}

	.section-title {
		font-size: 36rpx;
		font-weight: bold;
		color: #333;
		padding: 20rpx 10rpx 10rpx;
		margin-top: 20rpx;
		border-bottom: 2rpx solid #eee;
	}
	.section-title:first-child {
		margin-top: 0;
	}

	.last-updated-time {
		font-size: 24rpx;
		color: #888;
		text-align: right;
		padding: 0 10rpx 10rpx;
	}

	.dev-area {
		display: flex;
		justify-content: space-around;
		flex-wrap: wrap;
		gap: 20rpx;
		padding: 10rpx 0;
	}

	.dev-cart {
		height: 150rpx;
		width: calc(50% - 20rpx);
		min-width: 300rpx;
		background-color: #fff;
		border-radius: 20rpx;
		display: flex;
		justify-content: space-around;
		align-items: center;
		box-shadow: 0 4rpx 12rpx rgba(0,0,0,0.08);
		box-sizing: border-box;
		padding: 15rpx;
		font-weight: bold;
	}
	/* Style for breach count card (for master) */
	.breach-count-cart {
		flex-direction: column; /* Stack name and count vertically */
		justify-content: center;
	}
	.breach-count-cart .dev-name {
		margin-bottom: 5rpx; /* Space between name and count */
	}

	.alarm-cart {
		display: flex;
		justify-content: center;
		align-items: center;
		font-size: 32rpx;
		color: white;
	}


	.dev-name {
		font-size: 24rpx;
		text-align: center;
		color: #555;
		margin-bottom: 8rpx;
		font-weight: normal;
	}

	.dev-logo {
		width: 60rpx;
		height: 60rpx;
		margin-top: 5rpx;
		display: block;
		margin-left: auto;
		margin-right: auto;
	}

	.dev-data {
		font-size: 42rpx;
		color: #333;
		text-align: center;
		display: inline-block;
		vertical-align: baseline;
	}
	.count-data { /* Specific style for count numbers */
		font-size: 48rpx; /* Make count number larger */
		color: #E6A23C; /* Warning color for counts, adjust as needed */
	}


	.dev-data .unit-ppm {
		font-size: 24rpx;
		color: #666;
		margin-left: 5rpx;
		vertical-align: baseline;
		font-weight: normal;
	}

	/* New styles for slave summary card */
	.slave-summary-card {
		background-color: #fff;
		border-radius: 20rpx;
		padding: 20rpx;
		box-shadow: 0 4rpx 12rpx rgba(0,0,0,0.08);
		margin-bottom: 20rpx; /* Space below summary card */
	}
	.summary-title {
		font-size: 28rpx;
		font-weight: bold;
		color: #307EC7;
		margin-bottom: 15rpx;
		text-align: center;
	}
	.summary-grid {
		display: grid;
		grid-template-columns: repeat(auto-fit, minmax(280rpx, 1fr));
		gap: 10rpx;
	}
	.summary-item {
		background-color: #f9f9f9;
		padding: 10rpx 15rpx;
		border-radius: 10rpx;
		display: flex;
		justify-content: space-between;
		align-items: center;
	}
	.summary-label {
		font-size: 24rpx;
		color: #555;
	}
	.summary-value {
		font-size: 26rpx;
		color: #333;
		font-weight: 500;
	}


	.slave-devices-area {
		display: flex;
		flex-direction: column;
		gap: 20rpx;
		padding: 10rpx 0;
	}

	.slave-device-card {
		background-color: #fff;
		border-radius: 20rpx;
		padding: 20rpx;
		box-shadow: 0 4rpx 12rpx rgba(0,0,0,0.08);
	}
	
	.slave-device-header {
		display: flex;
		justify-content: space-between;
		align-items: center;
		margin-bottom: 15rpx;
		padding-bottom: 10rpx;
		border-bottom: 1rpx solid #f0f0f0;
	}

	.slave-device-name {
		font-size: 30rpx;
		font-weight: bold;
		color: #307EC7;
	}

	.slave-parameters {
		display: grid;
		grid-template-columns: repeat(auto-fit, minmax(280rpx, 1fr)); /* Adjust minmax for better fit if needed */
		gap: 15rpx;
		/* margin-bottom: 15rpx; /* This was removed to allow action bar to be directly below */
	}

	.slave-param-item {
		background-color: #f9f9f9;
		padding: 15rpx;
		border-radius: 10rpx;
		display: flex;
		justify-content: space-between;
		align-items: center;
	}
	.slave-param-item .param-value.count-data { /* Target count data within slave param item */
		font-size: 32rpx; /* Slightly smaller count for slaves, but still prominent */
		color: #E6A23C; /* Ensure slave count also uses the warning color */
	}

	.alarm-status-item {
		padding: 15rpx 20rpx;
	}
	.alarm-status-item .param-label {
		color: #fff; 
	}
	.alarm-status-item .alarm-text {
		font-weight: bold;
		color: #fff; 
	}

	.param-label {
		font-size: 26rpx;
		color: #555;
	}

	.param-value {
		font-size: 28rpx;
		color: #333;
		font-weight: 500;
	}

	/* Modified: Container for breach count and toggle button */
	.slave-actions-bar { 
		display: flex;
		justify-content: space-between; 
		align-items: center; /* Vertically align items in the middle */
		margin-top: 15rpx; 
		margin-bottom: 10rpx; 
		/* padding: 5rpx 0; /* Removed to let items define height */
	}
	.breach-count-inline-display { 
		display: flex;
		align-items: center;
		background-color: #f9f9f9; /* Same as .slave-param-item */
		padding: 10rpx 15rpx;     /* Adjusted vertical padding to better match button height */
		border-radius: 10rpx;   /* Same as .slave-param-item */
		/* margin-right: 10rpx;    /* REMOVED to allow space-between to work better */
		height: 60rpx; /* Explicit height to match mini button */
		box-sizing: border-box;
	}
	.action-bar-label { 
		font-size: 24rpx; /* Adjusted for balance */
		color: #555;     
		margin-right: 8rpx;
	}
	.action-bar-value.count-data { 
		font-size: 28rpx; /* Adjusted for balance */
		color: #E6A23C; 
		font-weight: bold; 
	}
	.toggle-button {
		flex-shrink: 0; /* Prevent button from shrinking */
		height: 60rpx; /* Match explicit height */
		line-height: 60rpx; /* For vertical text centering if text is single line */
		margin-left: 10rpx; /* Add some space if breach count is present */
	}


	.threshold-settings {
		margin-top: 10rpx; 
		padding-top: 15rpx;
		border-top: 1rpx solid #eee;
		background-color: #fdfdfd; 
		border-radius: 10rpx; 
		padding: 15rpx;
	}
	.threshold-item {
		display: flex;
		align-items: center;
		margin-bottom: 8rpx; 
		padding: 3rpx 0; 
	}
	.threshold-label {
		font-size: 22rpx; 
		color: #666;
		width: 200rpx; 
		flex-shrink: 0;
		margin-right: 10rpx;
	}
	.threshold-slider {
		flex-grow: 1;
	}
	.threshold-slider slider { 
		margin-left: 0 !important; 
	}


	.controls-area {
		display: flex;
		flex-direction: column;
		gap: 20rpx;
		padding: 10rpx 0;
	}
	.device-cart-l {
		height: auto;
		min-height: 120rpx;
		width: 100%;
		background-color: #fff;
		border-radius: 20rpx;
		display: flex;
		justify-content: space-between;
		align-items: center;
		box-shadow: 0 4rpx 12rpx rgba(0,0,0,0.08);
		padding: 20rpx 30rpx;
		box-sizing: border-box;
	}
	.device-cart-l .dev-name {
		font-size: 28rpx;
		color: #333;
		margin-bottom: 0;
		text-align: left;
		flex-shrink: 0;
		margin-right: 20rpx;
		font-weight: normal;
	}

	.ctrl-slider {
		width: auto;
		flex-grow: 1;
		margin-left: 10rpx;
	}

	switch {
		transform: scale(0.8);
	}

	.danger-bg {
		background-color: red !important; 
		color: white; 
	}
	.safe-bg {
		background-color: green !important; 
		color: white; 
	}
	.slave-safe-bg {
		background-color: #90ee90 !important; 
		color: #333; 
	}
	.slave-safe-bg .param-label, .slave-safe-bg .param-value {
		color: #333 !important; 
	}

	.danger-bg .param-label, .danger-bg .param-value,
	.safe-bg .param-label, .safe-bg .param-value {
		color: white; 
	}

</style>
