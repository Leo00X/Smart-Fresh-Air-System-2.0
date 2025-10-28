<template>
	<view class="container">
		<view class="panel-card-full">
			<uni-segmented-control :current="currentChartIndex" :values="chartItems" @clickItem="onTabChange"
				style-type="button" active-color="#007aff"></uni-segmented-control>
		</view>

		<view class="panel-card">
			<view class="panel-title">24小时平均值 - {{ chartItems[currentChartIndex] }}</view>
			
			<view class="chart-wrapper">
				<qiun-data-charts v-if="!isLoading" 
					type="column" 
					:opts="avgChartOptions"
					:chartData="avgChartData" 
					:error="chartError" />
					
				<view v-if="isLoading" class="chart-loading">
					<text>平均值计算中...</text>
				</view>
				
				<view v-if="!isLoading && chartError" class="chart-loading">
					<text style="color: red;">{{ chartError }}</text>
				</view>
			</view>
		</view>

		<view class="panel-card">
			<view class="panel-title">历史数据 (24小时) - {{ chartItems[currentChartIndex] }}</view>
			
			<view class="chart-wrapper">
				<qiun-data-charts v-if="!isLoading"
					type="line" 
					:opts="lineChartOptions"
					:chartData="lineChartData" 
					:error="chartError" />
					
				<view v-if="isLoading" class="chart-loading">
					<text>图表数据加载中...</text>
				</view>
				
				<view v-if="!isLoading && chartError" class="chart-loading">
					<text style="color: red;">{{ chartError }}</text>
				</view>
			</view>
		</view>
	</view>
</template>

<script>
	// 导入 uCharts 和 Token 生成器
	import qiunDataCharts from '@/uni_modules/qiun-data-charts/components/qiun-data-charts/qiun-data-charts.vue';
	import { createCommonToken } from '@/key.js'; 

	export default {
		components: {
			qiunDataCharts
		},
		data() {
			return {
				// --- 鉴权与凭证 (来自您的工作文件) ---
				PROID: "sMFCi53wHH",
				DEVICE_NAME: "air_01",
				token: '',
				apiGetHistory: 'https://iot-api.heclouds.com/thingmodel/query-device-property-history',
				
				// --- (!!新!!) 统一Tab ---
				currentChartIndex: 0, 
				chartItems: ['温度', '湿度', 'PM2.5', '甲醛', '烟雾', 'AQI/CO'], 
				
				// --- (!!新!!) 图表数据 ---
				isLoading: true, 
				chartError: null,
				
				// (!!新!!) 柱状图数据 (平均值)
				avgChartData: {},
				avgChartOptions: {
					yAxis: { data: [{ title: '', min: 0 }] },
					legend: { show: false }, // 平均值图表不需要图例
				},
				
				// (!!新!!) 曲线图数据 (历史)
				lineChartData: {}, 
				lineChartOptions: {
					yAxis: { data: [{ title: '', min: 0 }] },
					legend: { show: true },
					extra: { line: { type: 'curve' } }
				},
			};
		},
		// (!!关键!!) Token 生成 (来自您的工作文件)
		onLoad() {
			try {
				const params = {
					author_key: 'MtSNFVVvy2EtY4CWtbyku+HF0mugavWVkGTAcok3Nnp1/UM03xkkEZr+EBPFbPeR',
					version: '2022-05-01',
					user_id: '441225',
				}
				this.token = createCommonToken(params);
				console.log("Summary.vue: Token 已生成");
			} catch (error) {
				console.error("Token 生成失败:", error);
				uni.showToast({ title: 'Token生成失败', icon: 'error' });
			}
		},
		// (!!新!!) 页面显示时，加载当前Tab的数据
		onShow() {
			if (!this.token) {
				console.error("Token 为空, 无法获取数据");
				return;
			}
			this.loadDataForCurrentTab();
		},
		methods: {
			
			// ----------------------------------------------
			// ------ (!!新!!) 核心调度与数据处理 ------
			// ----------------------------------------------

			/**
			 * (!!关键修正!!) 修复了Tab点击无效的BUG
			 */
			onTabChange(e) {
				if (this.currentChartIndex !== e.currentIndex) {
					this.currentChartIndex = e.currentIndex;
					this.loadDataForCurrentTab(); // (!!BUG修复!!)
				}
			},
			
			/**
			 * (!!新!!) 统一的数据调度中心
			 */
			async loadDataForCurrentTab() {
				this.isLoading = true; 
				this.chartError = null; 
				this.avgChartData = {};  // 清空旧数据
				this.lineChartData = {}; // 清空旧数据
				
				let yAxisTitle = ''; 
				let identifiers = []; // 要查询的ID
				let nameMap = {};     // 图表名称映射
				
				// (!!新!!) 按单位(Tab)设置要查询的ID
				switch (this.currentChartIndex) {
					case 0: // 温度
						identifiers = ['host_temperature', 'slave1_temperature', 'slave2_temperature', 'slave3_temperature'];
						nameMap = {'host_temperature': '主机', 'slave1_temperature': '新风机①', 'slave2_temperature': '新风机②', 'slave3_temperature': '新风机③'};
						yAxisTitle = '温度 (°C)';
						break;
					case 1: // 湿度
						identifiers = ['host_humidity', 'slave1_humidity', 'slave2_humidity', 'slave3_humidity'];
						nameMap = {'host_humidity': '主机', 'slave1_humidity': '新风机①', 'slave2_humidity': '新风机②', 'slave3_humidity': '新风机③'};
						yAxisTitle = '湿度 (%)';
						break;
					case 2: // PM2.5
						identifiers = ['host_pm25_value'];
						nameMap = {'host_pm25_value': '主机'};
						yAxisTitle = 'PM2.5 (μg/m³)';
						break;
					case 3: // 甲醛
						identifiers = ['host_formaldehyde_ppb'];
						nameMap = {'host_formaldehyde_ppb': '主机'};
						yAxisTitle = '甲醛 (ppb)';
						break;
					case 4: // 烟雾
						identifiers = ['host_smog_ppm', 'slave1_smog_ppm', 'slave2_smog_ppm', 'slave3_smog_ppm'];
						nameMap = {'host_smog_ppm': '主机', 'slave1_smog_ppm': '新风机①', 'slave2_smog_ppm': '新风机②', 'slave3_smog_ppm': '新风机③'};
						yAxisTitle = '烟雾 (PPM)';
						break;
					case 5: // AQI/CO
						identifiers = ['slave1_aqi_ppm', 'slave2_aqi_ppm', 'slave3_aqi_ppm', 'slave1_co_ppm', 'slave2_co_ppm', 'slave3_co_ppm'];
						nameMap = {'slave1_aqi_ppm': '新风机① AQI', 'slave2_aqi_ppm': '新风机② AQI', 'slave3_aqi_ppm': '新风机③ AQI', 'slave1_co_ppm': '新风机① CO', 'slave2_co_ppm': '新风机② CO', 'slave3_co_ppm': '新风机③ CO'};
						yAxisTitle = 'AQI / CO (PPM)';
						break;
				}
				
				// 更新Y轴标题
				this.avgChartOptions.yAxis.data[0].title = yAxisTitle;
				this.lineChartOptions.yAxis.data[0].title = yAxisTitle;
				
				try {
					// 1. 并发获取所有需要的历史数据
					const promises = identifiers.map(id => this.getDeviceHistoryData(id));
					const results = await Promise.all(promises); 
					
					console.log("（API）所有历史数据获取成功", results);
					
					// 2. 检查是否完全没有数据
					const hasData = results.some(res => res.list && res.list.length > 0);
					if (!hasData) {
						throw new Error("该时段无数据");
					}

					// 3. (!!新!!) 生成平均值柱状图数据
					this.avgChartData = this.formatDataForAvgChart(results, nameMap);
					
					// 4. (!!新!!) 生成历史曲线图数据
					this.lineChartData = this.formatDataForLineChart(results, nameMap); 
					
					this.isLoading = false;
					
				} catch (err) {
					this.isLoading = false;
					this.chartError = `加载失败: ${err.message || err}`;
					console.error("（图表）数据加载失败:", err);
				}
			},

			/**
			 * 核心 - 获取[历史数据] (此函数逻辑正确, 无需修改)
			 */
			getDeviceHistoryData(identifier) {
				// console.log(`(API) 开始获取 ${identifier} ...`);
				
				const endTime = Date.now();
				const startTime = endTime - 24 * 3600 * 1000;
				
				const apiUrl = this.apiGetHistory;
				
				const params = {
					product_id: this.PROID,      
					device_name: this.DEVICE_NAME, 
					identifier: identifier,       
					start_time: startTime,          
					end_time: endTime,              
					limit: 100,
					sort: 1   
				};

				const requestHeader = {
					'authorization': this.token
				};
				
				return new Promise((resolve, reject) => {
					uni.request({
						url: apiUrl,
						method: 'GET',
						header: requestHeader,
						data: params,
						success: (res) => {
							if (res.statusCode === 200 && res.data.code === 0) {
								resolve({
									id: identifier,
									list: res.data.data.list || []
								}); 
							} else {
								reject(res.data.msg || '服务器错误');
							}
						},
						fail: (err) => {
							reject('网络请求失败');
						}
					});
				});
			},
			
			// ----------------------------------------------
			// ------ (!!新!!) 图表格式化函数 (已修复!!) ------
			// ----------------------------------------------

			/**
			 * (!!新!!) 辅助函数: 计算平均值
			 * (!!已按要求过滤0值!!)
			 * (!!已按要求格式化位数!!)
			 */
			calculateAverage(list) {
				if (!list || list.length === 0) {
					return 0; // 如果API返回空, 平均值为0
				}
				
				// 1. 先解析为数字, 再过滤掉无效值(如0或NaN)
				const validValues = list
					.map(p => parseFloat(p.value))
					.filter(v => v > 0); // (!!过滤掉0值!!)
				
				if (validValues.length === 0) {
					return 0; // 如果全是0或空, 平均值就是0
				}
				
				// 2. 计算
				const sum = validValues.reduce((a, b) => a + b, 0);
				const avg = sum / validValues.length;
				
				// 3. (!!关键修正!!) 返回保留一位小数的数字
				return parseFloat(avg.toFixed(1));
			},

			/**
			 * (!!新!!) 格式化为 uCharts 柱状图(平均值) 数据
			 */
			formatDataForAvgChart(apiResults, nameMap) {
				let categories = [];
				let data = [];
				
				apiResults.forEach(result => {
					// X轴: '主机', '新风机①'
					categories.push(nameMap[result.id] || result.id);
					// Y轴: [25.8, 24.5, ...]
					data.push(this.calculateAverage(result.list));
				});
				
				return {
					categories: categories,
					series: [{
						name: '24小时平均值', // (此名称不会显示, 因为 legend: false)
						data: data
					}]
				};
			},

			/**
			 * (!!新!!) 格式化为 uCharts 曲线图(历史) 数据
			 * (!!已修复0值直线BUG!!)
			 */
			formatDataForLineChart(apiResults, nameMap) {
				let categories = []; 
				let series = []; 
				
				// 1. 为每个ID创建一个序列
				apiResults.forEach(result => {
					series.push({
						name: nameMap[result.id] || result.id, // 使用新名称
						data: []
					});
				});

				// 2. 找到所有的时间点作为X轴
				let allTimePoints = new Set();
				apiResults.forEach(result => {
					if (result.list) {
						result.list.forEach(p => {
							allTimePoints.add(Number(p.time)); 
						});
					}
				});
				
				if (allTimePoints.size === 0) {
					console.warn("历史数据点为空");
					return { categories: [], series: [] };
				}

				// 3. 排序并格式化X轴
				let sortedTimePoints = Array.from(allTimePoints).sort((a, b) => a - b);
				categories = sortedTimePoints.map(ts => new Date(ts).toLocaleTimeString());

				// 4. 填充Y轴数据
				series.forEach((s, index) => {
					let stream = apiResults[index];
					let dataMap = new Map();
					if (stream.list) {
						stream.list.forEach(p => {
							// (!!关键修正!!) 过滤0值, 替换为 null
							let val = parseFloat(p.value);
							dataMap.set(Number(p.time), val > 0 ? val : null); 
						});
					}
					// 遍历X轴, 找不到的点自动为 null (uCharts会断线)
					sortedTimePoints.forEach(ts => {
						s.data.push(dataMap.get(ts) || null);
					});
				});
				
				return { categories, series };
			}
		}
	}
</script>

<style lang="scss">
	.container {
		display: flex;
		flex-direction: column;
		padding: 15px;
		background-color: #f8f8f8;
		min-height: 100vh;
		padding-bottom: 30px;
	}
	
	/* (!!新!!) 把Tab放在一个无padding的卡片中, 使其通栏 */
	.panel-card-full {
		background-color: #ffffff;
		border-radius: 8px;
		margin-bottom: 15px;
		box-shadow: 0 2px 8px rgba(0, 0, 0, 0.05);
		/* (!!新!!) 增加一点内边距, 让Tab和卡片边缘有呼吸感 */
		padding: 10px;
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
		margin-bottom: 20px;
		color: #333;
	}
	
	/* (!!新!!) 两个图表都需要这个样式 */
	.chart-wrapper {
		width: 100%;
		height: 250px; 
	}
	
	.chart-loading {
		display: flex;
		justify-content: center;
		align-items: center;
		height: 250px;
		color: #999;
		font-size: 14px;
	}
</style>