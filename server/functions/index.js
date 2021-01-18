'use strict';
const functions = require('firebase-functions');
const admin = require('firebase-admin');
const base64 = require('urlsafe-base64');

exports.fileupload = functions.region('asia-northeast1').https.onRequest(async (req, res) => {
  console.log('fileUpload function start!!');
  // CORS対応
  if (req.method === 'OPTIONS') {
    res.header('Access-Control-Allow-Origin', '*');
    res.header('Access-Control-Allow-Headers', 'Content-Type');
    res.header('Access-Control-Allow-Methods', 'POST');
    res.status(200).end();
    return;
  }

  if (req.method !== 'POST') {
    res.status(405).send('Method Not Allowed');
    return;
  }

  // 画像データ取得
  const b64img = req.rawBody;
  const buffer = base64.decode(b64img);

  console.log('buffer.length');
  console.log(buffer.length);

  // firebase admin接続
  let defaultApp;
  if (!admin.apps.length) {
    const serviceAccount = require('./viviware-dev2-firebase-adminsdk-3h2x9-4cb2338830.json');
    defaultApp = admin.initializeApp({
      credential: admin.credential.cert(serviceAccount),
    });
  } else {
    defaultApp = admin.apps[0];
  }

  // firebase storageのファイルを定義して取得
  const bucket = await admin.storage(defaultApp).bucket(functions.config().fileupload.bucket.name);
  const filePath = getFilePath();
  const fbStrageFile = bucket.file(`kashiwanoha/${filePath}`);

  const fileSaveErrors = await fbStrageFile.save(buffer, { metadata: { contentType: 'image/jpeg' } });

  if (fileSaveErrors && fileSaveErrors.length > 0) {
    console.log('Error!');
    console.error(fileSaveErrors);
    res.status(500).send(fileSaveErrors);
  } else {
    console.log('fileupload OK!');
    const url = await fbStrageFile.getSignedUrl({
      action: 'read',
      expires: '01-01-2099',
    });
    console.log(`url : ${url}`);
    res.status(200).send('fileupload OK');
  }
  return;
});

function getFilePath() {
  const unixTime = new Date().getTime() + 9 * 60 * 60 * 1000;
  const d = new Date(unixTime);
  const year = d.getFullYear();
  const month = d.getMonth() + 1;
  const day = d.getDate();
  const hour = ('0' + d.getHours()).slice(-2);
  const min = ('0' + d.getMinutes()).slice(-2);
  const sec = ('0' + d.getSeconds()).slice(-2);
  return `${year}/${month}/${day}/${hour}${min}${sec}.jpg`;
}
