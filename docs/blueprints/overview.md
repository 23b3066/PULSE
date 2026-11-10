# Blueprint調査：非対称協力を支える仕組み

PULSEでは、VR側が体験する状況と、現実側が得る情報・操作を組み合わせ、プレイヤー同士の会話を生み出すことを目指しています。以下はプロジェクト内のBlueprintアセットの名前や内部に保存された識別子から確認した内容です。**ノードの接続・実行順・値の変化はエディターや実機で未確認**のため、処理フローは断定していません。

## ハンドポーズによるVR操作 — `BP_VRPawn`

- **ゲーム上の狙い：** コントローラーを意識せず身体を使うことで、VR空間への没入感を高める。
- **確認した構成：** 左右の `HandPoseRecognizer`、`GetRecognizedHandPose`、認識名・信頼度に関する識別子、および `Teleport` に関する識別子があります。
- **確認が必要なこと：** 認識結果がどの操作につながるか、移動の条件、ハンドトラッキングが実際に有効になる設定。識別子の存在だけでは、これらの動作や精度は分かりません。
- **撮影候補：** 左右のハンドポーズ取得から操作へ分岐する箇所を全体表示し、認識結果とゲーム内の反応がつながる箇所を拡大する。

## センサー情報 — `BP_Serial`

- **ゲーム上の狙い：** 現実側の物理的な操作やプレイヤーの状態を、VR側との協力に結び付ける。
- **確認した構成：** `OpenComPort`、`ReceiveBPM`、`WeightEvent`、`SetWeight`、`BP_TileGimmick` と `WBP_BPM` への参照があります。心拍数については `CalculateMedianBPM`、`CalculateTrimmedBPM`、`CalibrationBPM`、`IsBPMSignalValid`、`HighBPMThreshold` などの識別子もあります。
- **読み取れる設計上の着眼点：** 心拍数の中央値や外れ値を除く処理、計測状態、基準値や閾値を扱うための名称が用意されています。実際にどの値をどの順序で使うかはグラフの確認が必要です。
- **確認が必要なこと：** 接続する機器、通信形式、重さとタイルの対応、心拍数によるHP・演出への影響、実機での安定性。
- **撮影候補：** ポート接続、BPM受信から補正・状態判定、重さ受信からゲーム側への受け渡しを、それぞれ別の画面で撮影する。ポート名や個人情報は隠す。

## 現実側の支援画面 — `WBP_RealSideMain`

- **ゲーム上の狙い：** 現実側が状況を把握し、適切なタイミングでVR側を支える。
- **確認した構成：** `Healing`、`Shield`、`Radar`、それぞれの回数やクールダウン、レーダーの範囲・更新間隔、2人分のBPM表示、ライフアイコン、進行度に関する識別子があります。`BP_Serial` と `BP_LevelProgressTracker` の参照もあります。
- **確認が必要なこと：** 各スキルの効果、消費や再使用条件、画面と他Blueprintの接続。
- **撮影候補：** Designerの画面全体と、回復・シールド・レーダーそれぞれのボタン入力からクールダウン管理までのグラフを個別に撮影する。

## タイルと進行 — `BP_TileGimmick` / `BP_Tile` / `BP_LevelProgressTracker`

- **ゲーム上の狙い：** 離れたプレイヤーが得た情報を交換し、障害を越えて進む。
- **確認した構成：** `BP_TileGimmick` にタイル位置、`BP_Tile` に生成されたタイルや削除に関する識別子があります。`BP_LevelProgressTracker` には開始・ゴール地点と進行率取得に関する識別子があります。
- **確認が必要なこと：** タイルの正しいルール、センサーからの入力との因果関係、進行率の計算とUIへの反映。
- **撮影候補：** タイルの配置とプレイ画面、`BP_Serial` から関連Blueprintに渡すグラフ、進行率取得のグラフを撮影する。

## 心拍数とライフ表示 — `WBP_BPM` / `WBP_LifeDisplay`

- **ゲーム上の狙い：** プレイヤーの状態を両者が認識し、緊張感や支援の判断につなげる。
- **確認した構成：** `WBP_BPM` に2人分のBPM表示と `FilteredBPM`、`BP_Serial` への参照があります。`WBP_LifeDisplay` にライフアイコン配列があります。
- **確認が必要なこと：** 共有HPの計算、心拍数による増減、表示更新のタイミング。
- **撮影候補：** BPMやライフが変化する前後の実機画面と、その値をUIへ反映するグラフを撮影する。

実際のグラフ画像は未掲載です。ノード接続の説明やスクリーンショットは、エディターで内容と公開権限を確認してから追加します。

## Blueprintファイルと依存関係

公開許可を得た次のアセットを、元ファイルのまま収録しています。GitHub上ではグラフを直接表示できず、Unreal Engineで開く場合も元の `/Game/PLUSE/` の配置と参照先が必要です。これらは完成したプロジェクトではありません。

| ファイル | 主な着眼点 |
| --- | --- |
| [BP_VRPawn](../../blueprints/PLUSE/BluePrint/BP_VRPawn.uasset) | 左右のハンドポーズ認識 |
| [BP_Serial](../../blueprints/PLUSE/BluePrint/BP_Serial.uasset) | シリアル通信、BPM、重さ |
| [BP_TileGimmick](../../blueprints/PLUSE/BluePrint/BP_TileGimmick.uasset) | タイル位置 |
| [BP_Tile](../../blueprints/PLUSE/BluePrint/BP_Tile.uasset) | タイルの生成・削除に関する識別子 |
| [BP_LevelProgressTracker](../../blueprints/PLUSE/BluePrint/BP_LevelProgressTracker.uasset) | 開始・ゴールと進行率 |
| [WBP_RealSideMain](../../blueprints/PLUSE/UI/WBP_RealSideMain.uasset) | 現実側のスキルと状態表示 |
| [WBP_BPM](../../blueprints/PLUSE/UI/WBP_BPM.uasset) | 2人分の心拍数表示 |
| [WBP_LifeDisplay](../../blueprints/PLUSE/UI/WBP_LifeDisplay.uasset) | ライフアイコン |

識別子・参照の調査では、OculusHandPoseRecognition／OculusXRInput、SERIALCOM、VR Templateの入力アクション、StarterContent、Dungeonのマテリアル、PULSE内の未収録Blueprint・UI・マテリアル等への参照を確認しました。これらのプラグインや素材は同梱していません。各アセットの実行順や参照先の完全な一覧は、エディターでの検証が必要です。
