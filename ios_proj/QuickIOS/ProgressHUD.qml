import QtQuick 2.2

Rectangle {
  id: progress
  width: 100
  height: 100
  color: "#a0000000"
  radius: 20

//  property bool animation: anim.running

  function start() {
    anim.start();
  }

  function stop() {
    anim.stop();
  }

  Canvas {
    id: canvas

    width: 60
    height: 60

    antialiasing: true

    anchors.centerIn: parent

    property real innerRadius: 0.8
    property real outerRadius: 1.0
    property real actualInnerRadius: 90
    property real actualOuterRadius: 100
    property color backgroundColor: "#46909090"
    property color foregroundColor: "#a0ffffff"

    property color primaryColor: "orange"
    property color secondaryColor: "blue"

    onPaint: {
      var size = Math.min(canvas.width, canvas.height);
      var coef = 0.5 * size;
      actualInnerRadius = coef * innerRadius;
      actualOuterRadius = coef * outerRadius;

      var ctx = getContext("2d");

      ctx.save();
      ctx.fillStyle = "#00000000";
      ctx.fillRect(0, 0, canvas.width, canvas.height);

      ctx.beginPath();
      ctx.fillRule = Qt.OddEvenFill;
      ctx.fillStyle = backgroundColor

      ctx.ellipse(0, 0, actualOuterRadius * 2.0, actualOuterRadius * 2.0);
      ctx.ellipse(actualOuterRadius - actualInnerRadius, actualOuterRadius - actualInnerRadius,
        actualInnerRadius * 2.0, actualInnerRadius * 2.0);

      ctx.fillStyle = backgroundColor;
      ctx.fill();

      var conical = ctx.createConicalGradient(actualOuterRadius, actualOuterRadius, 0.0);
      conical.addColorStop(0.0, "transparent");
      conical.addColorStop(0.05, foregroundColor);
      conical.addColorStop(0.8, "transparent");
      ctx.fillStyle = conical;
      ctx.fill();

      ctx.restore();
    }

    RotationAnimation {
      id: anim
      target: canvas
      property: "rotation"
      from: 0
      to: 360
      duration: 1200
      direction: RotationAnimation.Clockwise
      loops: Animation.Infinite
      running: false
    }
  }
}
