const express = require("express");
const app = express();
const fs = require("fs");
const port = process.argv[2]; // truyen port
const video_name = process.argv[3] //truyen vid name

// app.get("/", function (req, res) {
//   res.sendFile(__dirname + "/index.html");
// });

//check input params
if (port == undefined || port == null){
  console.log('no port define!!');
  process.exit(1);
}

if (video_name == undefined || video_name == null){
  console.log('no video define!!');
  process.exit(1);
}

app.get("/", function (req, res) {
  //temporary solution: terminate process after half a day?
  req.on('close', function(){
    setTimeout(function(){
      process.exit(0);
    }, 1000*60*60*12);
  });

  // Ensure there is a range given for the video
  const range = req.headers.range;
  if (!range) {
    res.status(400).send("Requires Range header");
  }

  // get video stats (about 61MB)
  const videoPath = video_name; //chỗ này cần phải tùy biến
  const videoSize = fs.statSync(videoPath).size;

  // Parse Range
  // Example: "bytes=32324-"
  const CHUNK_SIZE = 10 ** 6; // 1MB
  const start = Number(range.replace(/\D/g, ""));
  const end = Math.min(start + CHUNK_SIZE, videoSize - 1);

  // Create headers
  const contentLength = end - start + 1;
  const headers = {
    "Content-Range": `bytes ${start}-${end}/${videoSize}`,
    "Accept-Ranges": "bytes",
    "Content-Length": contentLength,
    "Content-Type": "video/mp4",
  };

  // HTTP Status 206 for Partial Content
  res.writeHead(206, headers);

  // create video read stream for this particular chunk
  const videoStream = fs.createReadStream(videoPath, { start, end });

  // Stream the video chunk to the client
  videoStream.pipe(res);
});

//port number nên để thành biến


app.listen(port, function () {
  console.log("Listening on port " + port + "!");
});

